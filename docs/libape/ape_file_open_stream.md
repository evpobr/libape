# ape_file_open_stream

Opens APE file from stream.

## Syntax

```c
APE_EXPORT int ape_file_open_stream(ape_stream *stream, ape_file **ctx);
```

### Parameters

`stream` **in**  
Initialized ape_stream structure.

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

[`ape_file_open`](ape_file_open.md)

[`ape_file_free`](ape_file_free.md)
