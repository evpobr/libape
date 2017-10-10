#ifndef APE_H
#define APE_H

/**
 * @file ape.h
 */

#include "ape_export.h"

#define LIBAPE_VERSION_STRING   "0.1.0"
#define LIBAPE_VERSION_MAYOR    0
#define LIBAPE_VERSION_MINOR    1
#define LIBAPE_VERSION_PATCH    0

#define APE_COMPRESSION_LEVEL_FAST          1000
#define APE_COMPRESSION_LEVEL_NORMAL        2000
#define APE_COMPRESSION_LEVEL_HIGH          3000
#define APE_COMPRESSION_LEVEL_EXTRA_HIGH    4000
#define APE_COMPRESSION_LEVEL_INSANE        5000

typedef struct ape_stream_s ape_stream;

typedef int (*ape_stream_read)(ape_stream *stream, void *buffer, int size);
typedef int (*ape_stream_write)(ape_stream *stream, const void *buffer, int size);
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
 * @param in stream Initialized ape_stream structure.
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
 * frame in Frame to return seek byte.
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
 * frame in Frame to return seek bit.
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
 * frame in Frame to return blocks count.
 * @return If function succeded, it returns blocks count,
 * otherwise result is zero or negative value.
 */
APE_EXPORT int ape_file_get_frame_blocks(ape_file *ctx, int frame);

#ifdef __cplusplus
}
#endif

#endif
