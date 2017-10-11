#ifndef APE_H
#define APE_H

/**
 * @file ape.h
 */

#include "ape_export.h"

/**
 * @brief Defines library version as string.
 *
 * String returned in "X.Y.Z" format, where X is major version number, Y is minor version number
 * and Z is patch version number.
 *
 * @see ape_get_version_string()
 */
#define LIBAPE_VERSION_STRING	"0.1.0"
/**
 * @brief Defines library major version.
 *
 * @see ape_get_version_major()
 */
#define LIBAPE_VERSION_MAYOR	0
/**
 * @brief Defines library minor version.
 *
 * @see ape_get_version_minor()
 */
#define LIBAPE_VERSION_MINOR	1
/**
 * @brief Defines library patch version.
 *
 * @see ape_get_version_patch()
 */
#define LIBAPE_VERSION_PATCH	0

/**
 * @brief Defines fast compression level.
 *
 * @see ape_file_get_compression_level()
 */
#define APE_COMPRESSION_LEVEL_FAST			1000
/**
 * @brief Defines normal compression level.
 *
 * @see ape_file_get_compression_level()
 */
#define APE_COMPRESSION_LEVEL_NORMAL		2000
/**
 * @brief Defines high compression level.
 *
 * @see ape_file_get_compression_level()
 */
#define APE_COMPRESSION_LEVEL_HIGH			3000
/**
 * @brief Defines extra high compression level.
 *
 * @see ape_file_get_compression_level()
 */
#define APE_COMPRESSION_LEVEL_EXTRA_HIGH	4000
/**
 * @brief Defines insane compression level.
 *
 * @see ape_file_get_compression_level()
 */
#define APE_COMPRESSION_LEVEL_INSANE		5000

/**
 *
 */
typedef struct ape_stream_s ape_stream;

/**
 * @brief Stream read function prototype.
 * @param stream in Pointer to ape_stream that specifies input stream.
 * @param buffer in Pointer to a block of memory of @c size bytes.
 * @param size in Size of buffer in bytes.
 * @see ape_stream
 * @see ape_file_open_stream()
 * @return Size of bytes read.
 */
typedef int (*ape_stream_read)(ape_stream *stream, void *buffer, int size);
/**
 * @brief Stream write function prototype.
 * @param stream in Pointer to ape_stream that specifies output stream.
 * @param buffer out Pointer to a block of memory of @c size bytes.
 * @param size in Size of buffer in bytes.
 * @see ape_stream
 * @see ape_file_open_stream()
 * @return Size of bytes written.
 */
typedef int (*ape_stream_write)(ape_stream *stream, const void *buffer, int size);
/**
 * @brief Stream tell function prototype.
 * @param stream in Pointer to ape_stream that specifies stream.
 * @see ape_stream
 * @see ape_file_open_stream()
 * @return Current position in stream.
 */
typedef long (*ape_stream_tell)(ape_stream *stream);
/**
 * @brief Stream seek function prototype.
 * @param stream in Pointer to ape_stream that specifies stream.
 * @param offset in Bytes count to seek.
 * @param origin in Seek method: @c 0 - from beginning, @c 1 - from current position,
 * @c 2 - from end (@c offset is negative in this case).
 * @see ape_stream
 * @see ape_file_open_stream()
 * @return Bytes count skipped.
 */
typedef long (*ape_stream_seek)(ape_stream *stream, long offset, int origin);

/**
 * @brief Describes APE stream.
 *
 * This struct is used in ape_file_open stream(). Define your own functions to read,
 * write, seek and tell stream using prototypes above, and assign to corresponding
 * fields of ape_stream_s. If you opened stream in read only mode, set @c is_readonly
 * to @c 1. @c user_data holds some associated value, e.g. @c FILE pointer, @c HFILE etc.
 *
 * @see ape_file_open_stream()
 * @see ape_stream_read
 * @see ape_stream_write
 * @see ape_stream_seek
 * @see ape_stream_tell
 */
struct ape_stream_s
{
	/**
	 * @brief Pointer to read function.
	 * @see ape_stream_read()
	 */
	ape_stream_read read;
	/**
	 * @brief Pointer to write function.
	 * @see ape_stream_write()
	 */
	ape_stream_write write;
	/**
	 * @brief Pointer to tell function.
	 * @see ape_stream_tell()
	 */
	ape_stream_tell tell;
	/**
	 * @brief Pointer to seek function.
	 * @see ape_stream_seek()
	 */
	ape_stream_seek seek;
	/**
	 * @brief Indicates that stream is read only, if set to @c 1.
	 */
	int is_readonly;
	/**
	 * @brief User associated data.
	 */
	void *user_data;
};

/**
 * @brief Describes APE context.
 *
 * Opaque type that describes APE object.
 *
 */
typedef struct ape_file_s ape_file;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets library version as string.
 * 
 * String returned in "X.Y.Z" format, where X is major version number, Y is minor version number
 * and Z is patch version number.
 *
 * @see ape_get_version_major()
 * @see ape_get_version_minor()
 * @see ape_get_version_patch()
 */
APE_EXPORT const char *  ape_get_version_string(void);

/**
 * @brief Gets library major version.
 *
 * @see ape_get_version_string()
 * @see ape_get_version_minor()
 * @see ape_get_version_patch()
 */
APE_EXPORT int ape_get_version_major(void);

/**
 * @brief Gets library minor version.
 *
 * @see ape_get_version_string()
 * @see ape_get_version_major()
 * @see ape_get_version_patch()
 */
APE_EXPORT int ape_get_version_minor(void);

/**
 * @brief Gets library patch version.
 *
 * @see ape_get_version_string()
 * @see ape_get_version_major()
 * @see ape_get_version_minor()
 */
APE_EXPORT int ape_get_version_patch(void);

/**
 * @brief Opens APE file from filename.
 *
 * ape_file_open() and ape_file_open_stream() functions are used to create APE context.
 *
 * If file is opened successfully, @c ctx parameter holds the APE context
 * of file, otherwise it is set to @c NULL. You can pass @c ctx to other functions to retrive information
 * about opened file.
 *
 * Use ape_file_free() function to close file and free associated memory.
 *
 * @param filename in Path to the file.
 * @param is_readonly in Set to @c 1 to open file in read only mode.
 * @param ctx out APE context.
 * @see ape_file_open_stream()
 * @see ape_file_free()
 * @return If function succedded @c ctx is valid APE context and return code is zero. 
 * If function fails, result is negative value and @c ctx is set to @c NULL. 
 */
APE_EXPORT int ape_file_open(const char *filename, int is_readonly, ape_file **ctx);

/**
 * @brief Opens APE file from stream.
 *
 * ape_file_open() and ape_file_open_stream() functions are used to create APE context.
 *
 * If file is opened successfully, @c ctx parameter holds the APE context
 * of file, otherwise it is set to @c NULL. You can pass @c ctx to other functions to retrive information
 * about opened file.
 *
 * Use ape_file_free() function to close file and free associated memory.
 *
 * @param stream in Initialized ape_stream structure.
 * @param ctx out APE context.
 * @see ape_file_open()
 * @see ape_file_free()
 * @return If function succedded @c ctx is valid APE context and return code is zero.
 * If function fails, result is negative value and @c ctx is set to @c NULL.
 */
APE_EXPORT int ape_file_open_stream(ape_stream *stream, ape_file **ctx);

/**
 * @brief Closes APE file.
 *
 * @param ctx in APE context.
 * @see ape_file_open()
 * @see ape_file_open_stream()
 * @return If function succedded return code is zero, otherwise result is negative value.
 */
APE_EXPORT int ape_file_free(ape_file *ctx);

/**
 * @brief Geta version of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns version of file as positive number, 
 * otherwise result is negative value.
 */
APE_EXPORT int ape_file_get_version(ape_file *ctx);

/**
 * @brief Gets compression level of APE file.
 *
 * Supported compression levels are APE_COMPRESSION_LEVEL_FAST, APE_COMPRESSION_LEVEL_NORMAL,
 * APE_COMPRESSION_LEVEL_HIGH, APE_COMPRESSION_LEVEL_EXTRA_HIGH and APE_COMPRESSION_LEVEL_INSANE.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns compression level of file as positive number,
 * otherwise result is negative value.
 */
APE_EXPORT int ape_file_get_compression_level(ape_file *ctx);

/**
 * @brief Gets total frames count of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns total frames count,
 * otherwise result is negative value.
 */
APE_EXPORT int ape_file_get_total_frames(ape_file *ctx);

/**
 * @brief Gets final frame blocks count of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns final frame blocks count,
 * otherwise result is negative value.
 */
APE_EXPORT int ape_file_get_final_frame_blocks(ape_file *ctx);

/**
 * @brief Gets blocks per frame of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns blocks per frame count,
 * otherwise result is negative value.
 */
APE_EXPORT int ape_file_get_blocks_per_frame(ape_file *ctx);

/**
 * @brief Gets channels count of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns channels count,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_channels(ape_file *ctx);

/**
 * @brief Gets samplerate of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns samplerate,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_samplerate(ape_file *ctx);

/**
 * @brief Gets bits per sample of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns bits per sample,
 * otherwise result is zero or negative value.
 * @see ape_file_get_bytes_per_sample()
 */
APE_EXPORT int ape_file_get_bits_per_sample(ape_file *ctx);

/**
 * @brief Gets bytes per sample of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns bytes per sample,
 * otherwise result is zero or negative value.
 * @see ape_file_get_bytes_per_sample()
 */
APE_EXPORT int ape_file_get_bytes_per_sample(ape_file *ctx);

/**
 * @brief Gets block align of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns block align,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_block_align(ape_file *ctx);

/**
 * @brief Gets total blocks count of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns total blocks count,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_total_blocks(ape_file *ctx);

/**
 * @brief Gets APE total blocks count of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns APE total blocks count,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_ape_total_bytes(ape_file *ctx);

/**
 * @brief Gets length of APE file in milliseconds.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns length of APE file in 
 * milliseconds, otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_length_ms(ape_file *ctx);

/**
 * @brief Gets average bitrate of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns average bitrate,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_average_bitrate(ape_file *ctx);

/**
 * @brief Gets decompressed bitrate of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns decompressed bitrate,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_decompressed_bitrate(ape_file *ctx);

/**
 * @brief Gets seek table elements count of APE file.
 *
 * @param ctx in APE context.
 * @return If function succeded, it returns seek table elements count,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_seek_table_elements(ape_file *ctx);

/**
 * @brief Gets seek byte of APE file.
 *
 * @param ctx in APE context.
 * @param frame in Frame to return seek byte.
 * @return If function succeded, it returns seek byte,
 * otherwise result is zero or negative value.
 * @see ape_file_get_seek_table_elements()
 * @see ape_file_get_seek_bit()
 */
APE_EXPORT int ape_file_get_seek_byte(ape_file *ctx, int frame);

/**
 * @brief Gets seek bit of APE file.
 *
 * @param ctx in APE context.
 * @param frame in Frame to return seek bit.
 * @return If function succeded, it returns seek bit,
 * otherwise result is zero or negative value.
 * @deprecated This feature is obsoleted in format version 3.80,
 * for any newer files @c 0 is returned, use ape_file_get_seek_byte() instead.
 * @see ape_file_get_seek_table_elements()
 * @see ape_file_get_seek_byte()
 */
APE_EXPORT char ape_file_get_seek_bit(ape_file *ctx, int frame);

/**
 * @brief Gets blocks count in frame of APE file.
 *
 * @param ctx in APE context.
 * @param frame in Frame to return blocks count.
 * @return If function succeded, it returns blocks count,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_frame_blocks(ape_file *ctx, int frame);

#ifdef __cplusplus
}
#endif

#endif
