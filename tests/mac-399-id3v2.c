#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <ape/ape.h>

int main(void)
{
    char *test_name = "mac-399-id3v2.ape";

    ape_file *ctx;

    char *cwd;
    cwd = getcwd(NULL, 0);

    printf("ape_info_test: current directory is: %s\n", cwd);
    printf("ape_info_test: test file: '%s'... ", test_name);

    int ret = ape_file_open(test_name, 1, &ctx);

    if (ret >= 0)
    {
        printf("opened\n");

        printf("\nape_info_test: file information:\n");

        int value = ape_file_get_version(ctx);
        printf("ape_info_test: version = %d - ", value);
        if (value == 3990)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_compression_level(ctx);
        printf("ape_info_test: compression level = %d - ", value);
        if (value == 3000)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_total_frames(ctx);
        printf("ape_info_test: total frames = %d - ", value);
        if (value == 3)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_final_frame_blocks(ctx);
        printf("ape_info_test: final frame blocks = %d - ", value);
        if (value == 9100)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_blocks_per_frame(ctx);
        printf("ape_info_test: blocks per frame = %d - ", value);
        if (value == 73728)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_channels(ctx);
        printf("ape_info_test: channels = %d - ", value);
        if (value == 2)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_samplerate(ctx);
        printf("ape_info_test: samplerate = %d - ", value);
        if (value == 44100)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_bits_per_sample(ctx);
        printf("ape_info_test: bits per sample = %d - ", value);
        if (value == 16)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_bytes_per_sample(ctx);
        printf("ape_info_test: bytes per sample = %d - ", value);
        if (value == 2)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_block_align(ctx);
        printf("ape_info_test: block align = %d - ", value);
        if (value == 4)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_total_blocks(ctx);
        printf("ape_info_test: total blocks = %d - ", value);
        if (value == 156556)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_ape_total_bytes(ctx);
        printf("ape_info_test: APE total bytes = %d - ", value);
        if (value == 89155)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_length_ms(ctx);
        printf("ape_info_test: length (ms) = %d - ", value);
        if (value == 3550)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_average_bitrate(ctx);
        printf("ape_info_test: average bitrate = %d - ", value);
        if (value == 200)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_decompressed_bitrate(ctx);
        printf("ape_info_test: decompressed bitrate = %d - ", value);
        if (value == 1411)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_seek_table_elements(ctx);
        printf("ape_info_test: seek table elements = %d - ", value);
        if (value == 7282)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_seek_byte(ctx, 0);
        printf("ape_info_test: seek byte of first frame = %d - ", value);
        if (value == 33147)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_seek_bit(ctx, 0);
        printf("ape_info_test: seek bit of first frame = %d - ", value);
        if (value < 0)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }

        value = ape_file_get_frame_blocks(ctx, 0);
        printf("ape_info_test: frame blocks of first frame = %d - ", value);
        if (value = 73728)
        {
            printf("ok.\n");
        }
        else
        {
            printf("fail.\n");
            ret = -1;
        }
    }
    else
    {
        printf("failed to open with code %d\n", ret);
    }

    if (ret >= 0)
        printf("ape_info_test: all tests passed\n");
    else
        printf("ape_info_test: failed with code %d\n", ret);

    ape_file_free(ctx);

    return ret;
}
