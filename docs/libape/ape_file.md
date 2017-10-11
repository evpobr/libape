# ape_file

Describes APE context.

## Syntax

```c
typedef struct ape_file_s ape_file;
```

## Remarks

Internal `ape_file_s` struct is hidden by this type to restrict access to its internal fields. To work with `ape_file` use other APE API functions.

[`ape_file_open`](ape_file_open.md) and [`ape_file_open_stream`](ape_file_open_stream.md) functions return pointer to this opaque type.

To destroy `ape_file` use [`ape_file_free`](ape_file_free.md) function, do not use standard `free` function.

## Example

```c
ape_file *ctx;

int ret = ape_file_open("file.ape", 0, &ctx);

ape_file_free(ctx);

```

## See also

[`ape_file_open`](ape_file_open.md), [`ape_file_open_stream`](ape_file_open_stream.md)

[`ape_file_free`](ape_file_free.md)
