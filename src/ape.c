#include <ape/ape.h>

#include "config.h"

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif
#include <memory.h>

static int ape_stream_stdio_read(ape_stream *stream, void *buffer, int size);
static int ape_stream_stdio_write(ape_stream *stream, const void *buffer, int size);
static long ape_stream_stdio_tell(ape_stream *stream);
static long ape_stream_stdio_seek(ape_stream *stream, long offset, int origin);

static const char ape_version_string[] = LIBAPE_VERSION_STRING;

APE_EXPORT const char * ape_get_version_string(void)
{
	return ape_version_string;
}

APE_EXPORT int ape_get_version_major(void)
{
	return LIBAPE_VERSION_MAYOR;
}

APE_EXPORT int ape_get_version_minor(void)
{
	return LIBAPE_VERSION_MINOR;
}

APE_EXPORT int ape_get_version_patch(void)
{
	return LIBAPE_VERSION_PATCH;
}

enum ape_format_flags_t
{
	APE_FORMAT_FLAG_8_BIT				= 1,
	APE_FORMAT_FLAG_CRC					= 2,
	APE_FORMAT_FLAG_HAS_PEAK_LEVEL		= 4,
	APE_FORMAT_FLAG_24_BIT				= 8,
	APE_FORMAT_FLAG_HAS_SEEK_ELEMENTS	= 16,
	APE_FORMAT_FLAG_CREATE_WAV_HEADER	= 32
};

struct ape_file_s
{
	ape_stream *stream;
	bool owns_stream;

	int version;
	int compression_level;
	enum ape_format_flags_t format_flags;
	int total_frames;
	int final_frame_blocks;
	int blocks_per_frame;
	int channels;
	int samplerate;
	int bits_per_sample;
	int bytes_per_sample;
	int block_align;
	int wav_header_bytes;
	int wav_data_bytes;
	int wav_terminating_bytes;
	int wav_total_bytes;
	int total_blocks;
	int ape_total_bytes;
	int length_ms;
	int average_bitrate;
	int decompressed_bitrate;
	int junk_header_bytes;
	int seek_table_elements;
	int *seek_byte_table;
	char *seek_bit_table;
	char *wav_header_data;
};

#pragma pack(push)
#pragma pack(4)

struct ape_file_descriptor_s
{
	char id[4];
	uint32_t version;
	uint32_t descriptor_bytes;
	uint32_t header_bytes;
	uint32_t seek_table_bytes;
	uint32_t header_data_bytes;
	uint32_t ape_frame_data_bytes;
	uint32_t ape_frame_data_bytes_high;
	uint32_t terminating_data_bytes;
	uint8_t  file_md5[16];
};

struct ape_header_v2_s
{
	uint16_t compression_level;
	uint16_t format_flags;
	uint32_t blocks_per_frame;
	uint32_t final_frame_blocks;
	uint32_t total_frames;

	uint16_t bits_per_sample;
	uint16_t channels;
	uint32_t samplerate;
};

struct ape_header_v1_s
{
	char id[4];
	uint16_t version;
	uint16_t compression_level;
	uint16_t format_flags;
	uint16_t channels;
	uint32_t samplerate;
	uint32_t header_bytes;
	uint32_t terminating_bytes;
	uint32_t total_frames;
	uint32_t final_frame_blocks;
};

struct wave_header_s
{
	uint8_t riff_header[4];
	uint32_t riff_bytes;
	uint8_t data_type_id[4];
	uint8_t format_header[4];
	uint32_t format_bytes;
	uint16_t format_tag;
	uint16_t channels;
	uint32_t samples_per_sec;
	uint32_t avg_bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample;
	char data_header[4];
	unsigned int data_bytes;
};

#pragma pack(pop)

static ape_file *ape_file_new(void);
static int ape_file_analyze(ape_file *ctx);

APE_EXPORT int ape_file_open(const char *filename, int is_readonly, ape_file **ctx)
{
	FILE *fp = NULL;
	ape_stream *stream = NULL;
	ape_file *c = NULL;
	int ret = -1;

	if (!ctx)
		return -EFAULT;
	*ctx = NULL;

	if (!is_readonly)
	{
		fp = fopen(filename, "rb+");
	}
	else
	{
		fp = fopen(filename, "rb");
	}

	if (!fp)
	{
		return -errno;
	}

	stream = calloc(1, sizeof(ape_stream));
	if (!stream)
	{
		fclose(fp);
		return -ENOMEM;
	}

	stream->read = ape_stream_stdio_read;
	stream->write = ape_stream_stdio_write;
	stream->tell = ape_stream_stdio_tell;
	stream->seek = ape_stream_stdio_seek;
	stream->user_data = (void*)fp;
	stream->is_readonly = is_readonly;

	c = ape_file_new();
	if (!c)
	{
		free(stream);
		fclose(fp);
		return -ENOMEM;
	}

	c->stream = stream;
	c->owns_stream = true;

	ret = ape_file_analyze(c);
	if (ret < 0)
	{
		ape_file_free(c);
		return ret;
	}

	*ctx = c;

	return 0;
}

APE_EXPORT int ape_file_open_stream(ape_stream *stream, ape_file **ctx)
{
	ape_file *c = NULL;

	if (!ctx)
		return -EFAULT;
	*ctx = NULL;

	if (!stream || (!stream->read) || (!stream->write) || (!stream->tell) || (!stream->seek))
		return -EINVAL;

	c = ape_file_new();
	if (!c)
		return -ENOMEM;

	c->stream = stream;

	*ctx = c;

	return 0;
};

APE_EXPORT int ape_file_free(ape_file *ctx)
{
	if (ctx)
	{
		if (ctx->owns_stream)
			if (ctx->stream)
			{
				if (ctx->stream->user_data)
					fclose((FILE*)ctx->stream->user_data);
				free(ctx->stream);
				if (ctx->seek_byte_table)
					free(ctx->seek_byte_table);
				if (ctx->seek_bit_table)
					free(ctx->seek_bit_table);
				if (ctx->wav_header_data)
					free(ctx->wav_header_data);
			}
	}

	return 0;
}


APE_EXPORT int ape_file_get_version(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->version;
}

APE_EXPORT int ape_file_get_compression_level(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->compression_level;
}

APE_EXPORT int ape_file_get_total_frames(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->total_frames;
}

APE_EXPORT int ape_file_get_final_frame_blocks(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->final_frame_blocks;
}

APE_EXPORT int ape_file_get_blocks_per_frame(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->blocks_per_frame;
}

APE_EXPORT int ape_file_get_channels(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->channels;
}

APE_EXPORT int ape_file_get_samplerate(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->samplerate;
}

APE_EXPORT int ape_file_get_bits_per_sample(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->bits_per_sample;
}

APE_EXPORT int ape_file_get_bytes_per_sample(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->bytes_per_sample;
}

APE_EXPORT int ape_file_get_block_align(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->block_align;
}

APE_EXPORT int ape_file_get_total_blocks(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->total_blocks;
}

APE_EXPORT int ape_file_get_ape_total_bytes(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->ape_total_bytes;
}

APE_EXPORT int ape_file_get_length_ms(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->length_ms;
}

APE_EXPORT int ape_file_get_average_bitrate(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->average_bitrate;
}

APE_EXPORT int ape_file_get_decompressed_bitrate(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->decompressed_bitrate;
}

APE_EXPORT int ape_file_get_seek_table_elements(ape_file *ctx)
{
	if (!ctx)
		return -EINVAL;

	return ctx->seek_table_elements;
}

APE_EXPORT int ape_file_get_seek_byte(ape_file *ctx, int frame)
{
	if (!ctx || !ctx->seek_byte_table)
		return -EINVAL;

	if ((frame < 0) || (frame > ctx->total_frames))
		return -ERANGE;

	return (ctx->seek_byte_table[frame] + ctx->junk_header_bytes);
}

APE_EXPORT char ape_file_get_seek_bit(ape_file *ctx, int frame)
{
	if (!ctx || !ctx->seek_bit_table)
		return -EINVAL;

	if (ctx->version > 3800)
		return 0;

	if ((frame < 0) || (frame > ctx->total_frames))
		return -ERANGE;

	return ctx->seek_bit_table[frame];
}

APE_EXPORT int ape_file_get_frame_blocks(ape_file *ctx, int frame)
{
	if (!ctx)
		return -EINVAL;

	if ((frame < 0) || (frame >= ctx->total_frames))
	{
		return -1;
	}
	else
	{
		if (frame != (ctx->total_frames - 1))
			return ctx->blocks_per_frame;
		else
			return ctx->final_frame_blocks;
	}
}

static int ape_stream_stdio_read(ape_stream *stream, void *ptr, int count)
{
	return (int)fread(ptr, (size_t)1, (size_t)count, (FILE*)stream->user_data);
}

static int ape_stream_stdio_write(ape_stream *stream, const void *ptr, int count)
{
	return (int)fwrite(ptr, (size_t)1, (size_t)count, (FILE*)stream->user_data);
}

static long ape_stream_stdio_tell(ape_stream *stream)
{
	return ftell((FILE*)stream->user_data);
}

static long ape_stream_stdio_seek(ape_stream *stream, long offset, int origin)
{
	return fseek((FILE*)stream->user_data, offset, origin);
}

static ape_file *ape_file_new(void)
{
	return calloc(sizeof(ape_file), 1);
}

static int ape_file_find_descriptor(ape_file *ctx, bool keep_position)
{
	int original_file_location = 0;
	int junk_bytes = 0;
	unsigned int bytes_read = 0;
	uint8_t id3v2_header[10];
	int scan_bytes = 0;
	uint32_t goad_id = (' ' << 24) | ('C' << 16) | ('A' << 8) | ('M');
	uint32_t real_id = 0;

	original_file_location = ctx->stream->tell(ctx->stream);
	ctx->stream->seek(ctx->stream, 0, SEEK_SET);

	junk_bytes = 0;

	bytes_read = 0;

	ctx->stream->read(ctx->stream, &id3v2_header, sizeof(id3v2_header));
	if (id3v2_header[0] == 'I' && id3v2_header[1] == 'D' && id3v2_header[2] == '3')
	{
		unsigned int sync_safe_length = 0;
		bool has_tag_footer = false;

		sync_safe_length = (id3v2_header[6] & 127) << 21;
		sync_safe_length += (id3v2_header[7] & 127) << 14;
		sync_safe_length += (id3v2_header[8] & 127) << 7;
		sync_safe_length += (id3v2_header[9] & 127);

		if (id3v2_header[5] & 16)
		{
			has_tag_footer = true;
			junk_bytes = sync_safe_length + 20;
		}
		else
		{
			junk_bytes = sync_safe_length + 10;
		}

		if (id3v2_header[5] & 64)
		{
		}

		ctx->stream->seek(ctx->stream, junk_bytes, SEEK_SET);

		if (!has_tag_footer)
		{
			uint8_t temp = 0;
			bytes_read = ctx->stream->read(ctx->stream, &temp, 1);
			while (temp == 0 && bytes_read == 1)
			{
				junk_bytes++;
				bytes_read = ctx->stream->read(ctx->stream, &temp, 1);
			}
		}
	}
	ctx->stream->seek(ctx->stream, junk_bytes, SEEK_SET);

	bytes_read = ctx->stream->read(ctx->stream, &real_id, sizeof(real_id));
	if (bytes_read != sizeof(real_id))
		return -EIO;

	bytes_read = 1;
	while ((goad_id != real_id) && (bytes_read == 1) && (scan_bytes < (1024 * 1024)))
	{
		uint8_t temp;
		bytes_read = ctx->stream->read(ctx->stream, &temp, 1);
		real_id = (((uint32_t)temp) << 24) | (real_id >> 8);
		junk_bytes++;
		scan_bytes++;
	}

	if (goad_id != real_id)
		junk_bytes = -1;

	if (keep_position && (junk_bytes != -1))
		ctx->stream->seek(ctx->stream, junk_bytes, SEEK_SET);
	else
		ctx->stream->seek(ctx->stream, original_file_location, SEEK_SET);

	return junk_bytes;
}

static int ape_file_analyze(ape_file *ctx)
{
	int ret = 0;
	char id[4];
	int bytes_read;
	long old_position = 0;
	long size = 0;

	old_position = ctx->stream->tell(ctx->stream);
	ctx->stream->seek(ctx->stream, 0, SEEK_END);
	size = ctx->stream->tell(ctx->stream);
	ctx->stream->seek(ctx->stream, 0, SEEK_SET);

	ctx->junk_header_bytes = ape_file_find_descriptor(ctx, true);
	if (ctx->junk_header_bytes < 0)
		return -EILSEQ;

	bytes_read = ctx->stream->read(ctx->stream, &id, sizeof(id));
	if (memcmp(id, "MAC ", 4) == 0)
	{
		uint32_t version;

		bytes_read = ctx->stream->read(ctx->stream, &version, sizeof(version));
		if (bytes_read != sizeof(version))
		{
			ret = -EIO;
			goto cleanup;
		}

		ctx->stream->seek(ctx->stream, ctx->junk_header_bytes, SEEK_SET);

		if (version >= 3980)
		{

			struct ape_file_descriptor_s ape_descriptor = { 0 };
			struct ape_header_v2_s header = { 0 };

			bytes_read = ctx->stream->read(ctx->stream, &ape_descriptor, sizeof(struct ape_file_descriptor_s));
			if (bytes_read != sizeof(struct ape_file_descriptor_s))
			{
				ret = -EIO;
				goto cleanup;
			}

			if (ape_descriptor.descriptor_bytes > sizeof(struct ape_file_descriptor_s))
			{
				ctx->stream->seek(ctx->stream, ape_descriptor.descriptor_bytes - sizeof(struct ape_file_descriptor_s), SEEK_CUR);
			}

			bytes_read = ctx->stream->read(ctx->stream, &header, sizeof(struct ape_header_v2_s));
			if (bytes_read != sizeof(struct ape_header_v2_s))
			{
				ret = -EIO;
				goto cleanup;
			}

			ctx->version = (int)version;
			ctx->compression_level = (int)header.compression_level;
			ctx->format_flags = (int)header.format_flags;
			ctx->total_frames = (int)header.total_frames;
			ctx->final_frame_blocks = (int)header.final_frame_blocks;
			ctx->blocks_per_frame = (int)header.blocks_per_frame;
			ctx->channels = (int)header.channels;
			ctx->samplerate = (int)header.samplerate;
			ctx->bits_per_sample = (int)header.bits_per_sample;
			ctx->bytes_per_sample = ctx->bits_per_sample / 8;
			ctx->block_align = ctx->bytes_per_sample * ctx->channels;
			ctx->total_blocks = (header.total_frames == 0) ? 0 : ((header.total_frames - 1) * ctx->blocks_per_frame) + header.final_frame_blocks;
			ctx->wav_header_bytes = (header.format_flags & APE_FORMAT_FLAG_CREATE_WAV_HEADER) ? sizeof(struct wave_header_s) : ape_descriptor.header_data_bytes;
			ctx->wav_terminating_bytes = ape_descriptor.terminating_data_bytes;
			ctx->wav_data_bytes = ctx->total_blocks * ctx->block_align;
			ctx->wav_total_bytes = ctx->wav_data_bytes + ctx->wav_header_bytes + ctx->wav_terminating_bytes;
			ctx->ape_total_bytes = size;
			ctx->length_ms = (int)(((double)ctx->total_blocks * (double)1000) / (double)ctx->samplerate);
			ctx->average_bitrate = (ctx->length_ms <= 0) ? 0 : (int)(((double)(ctx->ape_total_bytes) * (double)8) / (double)(ctx->length_ms));
			ctx->decompressed_bitrate = (ctx->block_align * ctx->samplerate * 8) / 1000;
			ctx->seek_table_elements = ape_descriptor.seek_table_bytes / 4;

			ctx->seek_byte_table = calloc(ctx->seek_table_elements, 4);
			if (!ctx->seek_byte_table)
			{
				ret = -ENOMEM;
				goto cleanup;
			}
			bytes_read = ctx->stream->read(ctx->stream, ctx->seek_byte_table, ctx->seek_table_elements * 4);
			if (bytes_read != ctx->seek_table_elements * 4)
			{
				ret = -EIO;
				goto cleanup;
			}

			if (!(header.format_flags & APE_FORMAT_FLAG_CREATE_WAV_HEADER))
			{
				ctx->wav_header_data = calloc(1, ctx->wav_header_bytes);
				if (!ctx->wav_header_data)
				{
					ret = -ENOMEM;
					goto cleanup;
				}
				bytes_read = ctx->stream->read(ctx->stream, ctx->wav_header_data, ctx->wav_header_bytes);
				if (bytes_read != ctx->seek_table_elements * 4)
				{
					ret = -EIO;
					goto cleanup;
				}
			}
		}
		else
		{
			struct ape_header_v1_s header = { 0 };

			bytes_read = ctx->stream->read(ctx->stream, &header, sizeof(struct ape_header_v1_s));
			if (bytes_read != sizeof(struct ape_header_v1_s))
			{
				ret = -EIO;
				goto cleanup;
			}

			if (header.total_frames == 0)
			{
				ret = -EILSEQ;
				goto cleanup;
			}

			if (header.format_flags & APE_FORMAT_FLAG_HAS_PEAK_LEVEL)
			{
				uint32_t peak_level = -1;

				bytes_read = ctx->stream->read(ctx->stream, &peak_level, sizeof(uint32_t));
				if (bytes_read != sizeof(uint32_t))
				{
					ret = -EIO;
					goto cleanup;
				}
			}

			if (header.format_flags & APE_FORMAT_FLAG_HAS_SEEK_ELEMENTS)
			{
				uint32_t seek_table_elements = 0;

				bytes_read = ctx->stream->read(ctx->stream, &seek_table_elements, sizeof(uint32_t));
				if (bytes_read != sizeof(uint32_t))
				{
					ret = -EIO;
					goto cleanup;
				}
				else
				{
					ctx->seek_table_elements = seek_table_elements;
				}
			}
			else
			{
				ctx->seek_table_elements = header.total_frames;
			}


			ctx->version = (int)header.version;
			ctx->compression_level = (int)header.compression_level;
			ctx->format_flags = (int)header.format_flags;
			ctx->total_frames = (int)header.total_frames;
			ctx->final_frame_blocks = (int)header.final_frame_blocks;
			ctx->blocks_per_frame = ((header.version >= 3900) || ((header.version >= 3800) && (header.compression_level == APE_COMPRESSION_LEVEL_EXTRA_HIGH))) ? 73728 : 9216;
			if ((header.version >= 3950))
				ctx->blocks_per_frame = 73728 * 4;
			ctx->channels = (int)header.channels;
			ctx->samplerate = (int)header.samplerate;
			ctx->bits_per_sample = (ctx->format_flags & APE_FORMAT_FLAG_8_BIT) ? 8 : ((ctx->format_flags & APE_FORMAT_FLAG_24_BIT) ? 24 : 16);
			ctx->bytes_per_sample = ctx->bits_per_sample / 8;
			ctx->block_align = ctx->bytes_per_sample * ctx->channels;
			ctx->total_blocks = (header.total_frames == 0) ? 0 : ((header.total_frames - 1) * ctx->blocks_per_frame) + header.final_frame_blocks;
			ctx->wav_header_bytes = (header.format_flags & APE_FORMAT_FLAG_CREATE_WAV_HEADER) ? sizeof(struct wave_header_s) : header.header_bytes;
			ctx->wav_terminating_bytes = (int)header.terminating_bytes;
			ctx->wav_data_bytes = ctx->total_blocks * ctx->block_align;
			ctx->wav_total_bytes = ctx->wav_data_bytes + ctx->wav_header_bytes + ctx->wav_terminating_bytes;
			ctx->ape_total_bytes = size;
			ctx->length_ms = (int)(((double)ctx->total_blocks * (double)1000) / (double)ctx->samplerate);
			ctx->average_bitrate = (ctx->length_ms <= 0) ? 0 : (int)((double)ctx->ape_total_bytes * (double)8 / (double)ctx->length_ms);
			ctx->decompressed_bitrate = (ctx->block_align * ctx->samplerate * 8) / 1000;

			if (!(header.format_flags & APE_FORMAT_FLAG_CREATE_WAV_HEADER))
			{
				ctx->wav_header_data = calloc(header.header_bytes, 1);
				if (!ctx->wav_header_data)
				{
					ret = -ENOMEM;
					goto cleanup;
				}
				bytes_read = ctx->stream->read(ctx->stream, ctx->wav_header_data, ctx->wav_header_bytes);
				if (bytes_read != ctx->wav_header_bytes)
				{
					ret = -EIO;
					goto cleanup;
				}
			}

			ctx->seek_byte_table = calloc(ctx->seek_table_elements, 4);
			if (!ctx->seek_byte_table)
			{
				ret = -ENOMEM;
				goto cleanup;
			}
			bytes_read = ctx->stream->read(ctx->stream, ctx->seek_byte_table, ctx->seek_table_elements * 4);
			if (bytes_read != ctx->seek_table_elements * 4)
			{
				ret = -EIO;
				goto cleanup;
			}

			if (header.version <= 3800)
			{
				ctx->seek_bit_table = calloc(ctx->seek_table_elements, 1);
				if (!ctx->seek_bit_table)
				{
					ret = -ENOMEM;
					goto cleanup;
				}

				bytes_read = ctx->stream->read(ctx->stream, ctx->seek_bit_table, ctx->seek_table_elements);
				if (bytes_read != ctx->seek_table_elements)
				{
					ret = -EIO;
					goto cleanup;
				}
			}

		}
	}
	else
	{
		ret = -EILSEQ;
	}

	goto success;

cleanup:
	if (ctx->seek_byte_table)
		free(ctx->seek_byte_table);
	if (ctx->seek_bit_table)
		free(ctx->seek_byte_table);
	if (ctx->wav_header_data)
		free(ctx->wav_header_data);

success:

	ctx->stream->seek(ctx->stream, old_position, SEEK_SET);

	return ret;
}
