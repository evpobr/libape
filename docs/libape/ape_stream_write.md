# ape_stream_write

Stream write function prototype.

## Syntax

```c
typedef int (*ape_stream_write)(ape_stream *stream, const void *buffer, int size);
```

### Parameters

`stream` **in**  
Pointer to [ape_stream](ape_stream_s.md) that specifies output stream.

`buffer` **out**  
Pointer to a block of memory of `size` bytes.

`size` **in**  
Size of buffer in bytes.

## Example

See [`ape_stream`](ape_stream_s.md).

## Return value

Size of bytes written.

## See also

[`ape_stream_read`](ape_stream_read.md), [`ape_stream_tell`](ape_stream_tell.md), [`ape_stream_seek`](ape_stream_seek.md)

[`ape_file_open_stream`](ape_file_open_stream.md)
