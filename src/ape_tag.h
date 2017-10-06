#ifndef APE_TAG_H
#define APE_TAG_H

typedef struct ape_tag_s ape_tag;

int ape_tag_analyze_stream(ape_stream *stream, ape_tag **tag);

#endif
