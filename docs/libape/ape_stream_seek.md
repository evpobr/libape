# ape_stream_seek

Stream seek function prototype.

## Syntax

```c
typedef long (*ape_stream_seek)(ape_stream *stream, long offset, int origin);
```

### Parameters

`stream` **in**  
Pointer to [ape_stream](ape_stream_s.md) that specifies stream.

`offset` **in**
Bytes count to seek.

`origin` **in**  
Seek method: `0` - from beginning, `1` - from current position, `2` - from end (`offset` is negative in this case).

## Example

See [`ape_stream`](ape_stream_s.md).

## Return value

Bytes count skipped.

## See also

[`ape_stream_read`](ape_stream_read.md), [`ape_stream_write`](ape_stream_write.md), [`ape_stream_tell`](ape_stream_tell.md)

[`ape_file_open_stream`](ape_file_open_stream.md)
