#include <ape/ape.h>
#include "ape_tag.h"

#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#define APE_TAG_FLAG_CONTAINS_HEADER    (1 << 31)
#define APE_TAG_FLAG_CONTAINS_FOOTER    (1 << 30)
#define APE_TAG_FLAG_IS_HEADER          (1 << 29)
#define APE_TAG_FLAGS_DEFAULT           (APE_TAG_FLAG_CONTAINS_FOOTER)

struct id3_tag_s
{
    char header[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[29];
    uint8_t track;
    uint8_t genre;
};

struct ape_tag_footer_s
{
    uint8_t id[8];
    uint32_t version;
    uint32_t size;
    uint32_t fields;
    uint32_t flags;
    uint8_t reserved[8];
};

struct ape_tag_s
{
    struct ape_tag_footer_s footer;
    char *fields;
};

int ape_tag_analyze_stream(ape_stream *stream, ape_tag **tag)
{
    if (!stream || !tag)
        return -EINVAL;

    long original_position = stream->tell(stream);
    stream->seek(stream, -(long)sizeof(struct id3_tag_s), SEEK_END);

    bool has_id3_tag = false;
    struct id3_tag_s id3_tag = { 0 };
    int bytes_read = stream->read(stream, &id3_tag, sizeof(struct id3_tag_s));
    if (bytes_read != sizeof(struct id3_tag_s))
        return -EIO;

    if (memcmp(&id3_tag.header, "TAG ", 4) == 0)
        has_id3_tag = true;

    struct ape_tag_footer_s ape_tag_footer = { 0 };
    long offset = sizeof(struct ape_tag_footer_s);
    if (has_id3_tag)
        offset = sizeof(struct id3_tag_s);
    stream->seek(stream, -offset, SEEK_END);
    bytes_read = stream->read(stream, &ape_tag_footer, sizeof(struct ape_tag_footer_s));
    if (bytes_read != sizeof(struct ape_tag_footer_s))
        return -EIO;

    if (memcmp(&ape_tag_footer.id, "APETAGEX", sizeof(ape_tag_footer.id)) == 0)
    {
        struct ape_tag_s *t = calloc(1, sizeof(ape_tag));
        if (!tag)
            return -ENOMEM;

        memcpy(&t->footer, &ape_tag_footer, sizeof(struct ape_tag_footer_s));
    }

    return 0;
}
