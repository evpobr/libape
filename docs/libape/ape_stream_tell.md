# ape_stream_tell

Stream tell function prototype.

## Syntax

```c
typedef long (*ape_stream_tell)(ape_stream *stream);
```

### Parameters

`stream` **in**  
Pointer to [ape_stream](ape_stream_s.md) that specifies stream.

## Example

See [`ape_stream`](ape_stream_s.md).

## Return value

Current position in stream.

## See also

[`ape_stream_read`](ape_stream_read.md), [`ape_stream_write`](ape_stream_write.md), [`ape_stream_seek`](ape_stream_seek.md)

[`ape_file_open_stream`](ape_file_open_stream.md)

