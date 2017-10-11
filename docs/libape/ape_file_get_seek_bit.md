# ape_file_get_seek_bit

Gets seek bit of APE file.

## Syntax

```c
APE_EXPORT char ape_file_get_seek_bit(ape_file *ctx, int frame);
```

### Parameters

`ctx` **in**   
APE context.

`frame` **in**  
Frame to return seek bit.

## Return value

Return value >= `0` indicates success, error otherwise.

## See also

[`ape_file_get_seek_table_elements`](ape_file_get_seek_table_elements.md)

[`ape_file_get_seek_byte`](ape_file_get_seek_byte.md)
