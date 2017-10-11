# ape_stream_s

Describes APE stream.

## Syntax

```c
typedef struct ape_stream_s ape_stream;

struct ape_stream_s
{
	ape_stream_read read;
	ape_stream_write write;
	ape_stream_tell tell;
	ape_stream_seek seek;
	int is_readonly;
	void *user_data;
};
```

### Data fields

`read`  
Pointer to [read function](ape_stream_read.md).

`write`  
Pointer to [write function](ape_stream_write.md).

`tell`  
Pointer to [tell function](ape_stream_tell.md).

`seek`  
Pointer to [seek function](ape_stream_seek.md).

`is_readonly`  
Indicates that stream is read only, if set to `1`.
 
`user_data`  
User associated data.

## Remarks

This struct is used in [`ape_file_open_stream`](ape_file_open_stream.md) function. Define your own functions to read, write, seek and tell stream using `ape_stream_xxx` prototypes, and assign to corresponding fields of `ape_stream_s`. If you opened stream in read only mode, set `is_readonly` to `1`. `user_data` holds some associated value, e.g. `FILE` pointer, `HFILE` etc.

## Example

```c
int ape_stream_stdio_read(ape_stream *stream, void *ptr, int count)
{
	return (int)fread(ptr, (size_t)1, (size_t)count, (FILE*)stream->user_data);
}

int ape_stream_stdio_write(ape_stream *stream, const void *ptr, int count)
{
	return (int)fwrite(ptr, (size_t)1, (size_t)count, (FILE*)stream->user_data);
}

long ape_stream_stdio_tell(ape_stream *stream)
{
	return ftell((FILE*)stream->user_data);
}

long ape_stream_stdio_seek(ape_stream *stream, long offset, int origin)
{
	return fseek((FILE*)stream->user_data, offset, origin);
}

void open_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp)
    {
        struct ape_steam_s stream;

        stream->read = ape_stream_stdio_read;
        stream->write = ape_stream_stdio_write;
        stream->tell = ape_stream_stdio_tell;
        stream->seek = ape_stream_stdio_seek;
        stream->user_data = (void*)fp;
        stream->is_readonly = 1;

        ape_file *ctx;

        int ret = ape_file_open_stream(&stream, &ctx);
        if (ret >= 0)
        {
            ape_file_free(ctx);
        }

        fclose(fp);
    }
```

## See also

[`ape_stream_read`](ape_stream_read.md), [`ape_stream_write`](ape_stream_write.md), [`ape_stream_tell`](ape_stream_tell.md), [`ape_stream_seek`](ape_stream_seek.md)

[`ape_file_open_stream`](ape_file_open_stream.md)
