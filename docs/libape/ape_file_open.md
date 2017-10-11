# ape_file_open

Opens APE file from filename.

## Syntax

```c
APE_EXPORT int ape_file_open(const char *filename, int is_readonly, ape_file **ctx);
```

### Parameters

`filename` **in**  
Path to the file.

`is_readonly` **in**  
Set to `1` to open file in read only mode.

`ctx` **out**  
APE context.

## Remarks

`ape_file_open` is used to create APE context. If file is opened successfully, `ctx` parameter holds the APE context of file, otherwise it is set to `NULL`. You can pass `ctx` to other functions to retrive information about opened file.

## Example

See [`ape_file`](ape_file.md).

## Return value

Return value >= `0` indicates success, error otherwise.

## See also

[`ape_file`](ape_file.md)

[`ape_file_open_stream`](ape_file_open_stream.md)

[`ape_file_free`](ape_file_free.md)
