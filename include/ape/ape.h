#ifndef APE_H
#define APE_H

#include "ape_export.h"

#define LIBAPE_VERSION_STRING   "0.1.0"
#define LIBAPE_VERSION_MAYOR    0
#define LIBAPE_VERSION_MINOR    1
#define LIBAPE_VERSION_PATCH    0

enum ape_compression_level_t
{
    APE_COMPRESSION_LEVEL_FAST          = 1000,
    APE_COMPRESSION_LEVEL_NORMAL        = 2000,
    APE_COMPRESSION_LEVEL_HIGH          = 3000,
    APE_COMPRESSION_LEVEL_EXTRA_HIGH    = 4000,
    APE_COMPRESSION_LEVEL_INSANE        = 5000
};

enum ape_format_flags_t
{
    APE_FORMAT_FLAG_8_BIT               = 1,
    APE_FORMAT_FLAG_CRC                 = 2,
    APE_FORMAT_FLAG_HAS_PEAK_LEVEL      = 4,
    APE_FORMAT_FLAG_24_BIT              = 8,
    APE_FORMAT_FLAG_HAS_SEEK_ELEMENTS   = 16,
    APE_FORMAT_FLAG_CREATE_WAV_HEADER   = 32
};

APE_EXPORT const char *  ape_get_version_string(void);
APE_EXPORT int ape_get_version_major(void);
APE_EXPORT int ape_get_version_minor(void);
APE_EXPORT int ape_get_version_patch(void);

typedef struct ape_stream_s ape_stream;

typedef int (*ape_stream_read)(ape_stream *stream, void *ptr, int count);
typedef int (*ape_stream_write)(ape_stream *stream, const void *ptr, int count);
typedef long (*ape_stream_tell)(ape_stream *stream);
typedef long (*ape_stream_seek)(ape_stream *stream, long offset, int origin);

struct ape_stream_s
{
    ape_stream_read read;
    ape_stream_write write;
    ape_stream_tell tell;
    ape_stream_seek seek;
    int is_readonly;
    void *user_data;
};

typedef struct ape_file_s ape_file;

#ifdef __cplusplus
extern "C" {
#endif

APE_EXPORT int ape_file_open(const char *filename, int is_readonly, ape_file **ctx);
APE_EXPORT int ape_file_open_stream(ape_stream *stream, ape_file **ctx);
APE_EXPORT int ape_file_free(ape_file *ctx);
APE_EXPORT ape_file * ape_file_ref(ape_file *ctx);
APE_EXPORT ape_file * ape_file_unref(ape_file *ctx);
APE_EXPORT int ape_file_get_version(ape_file *ctx);
APE_EXPORT int ape_file_get_compression_level(ape_file *ctx);
APE_EXPORT enum ape_format_flags_t ape_file_get_format_flags(ape_file *ctx);
APE_EXPORT int ape_file_get_total_frames(ape_file *ctx);
APE_EXPORT int ape_file_get_final_frame_blocks(ape_file *ctx);
APE_EXPORT int ape_file_get_blocks_per_frame(ape_file *ctx);
APE_EXPORT int ape_file_get_channels(ape_file *ctx);
APE_EXPORT int ape_file_get_samplerate(ape_file *ctx);
APE_EXPORT int ape_file_get_bits_per_sample(ape_file *ctx);
APE_EXPORT int ape_file_get_bytes_per_sample(ape_file *ctx);
APE_EXPORT int ape_file_get_block_align(ape_file *ctx);
APE_EXPORT int ape_file_get_total_blocks(ape_file *ctx);
APE_EXPORT int ape_file_get_ape_total_bytes(ape_file *ctx);
APE_EXPORT int ape_file_get_length_ms(ape_file *ctx);
APE_EXPORT int ape_file_get_average_bitrate(ape_file *ctx);
APE_EXPORT int ape_file_get_decompressed_bitrate(ape_file *ctx);
APE_EXPORT int ape_file_get_seek_table_elements(ape_file *ctx);
APE_EXPORT int ape_file_get_seek_byte(ape_file *ctx, int frame);
APE_EXPORT char ape_file_get_seek_bit(ape_file *ctx, int frame);
APE_EXPORT int ape_file_get_frame_blocks(ape_file *ctx, int frame);

#ifdef __cplusplus
}
#endif

#endif
