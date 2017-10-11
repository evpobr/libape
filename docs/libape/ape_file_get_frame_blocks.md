# ape_file_get_frame_blocks

Gets blocks count in frame of APE file.

## Syntax

```c
APE_EXPORT int ape_file_get_frame_blocks(ape_file *ctx, int frame);
```

### Parameters

`ctx` **in**   
APE context.

`frame` **in**  
Frame to return blocks count.

## Return value

Return value >= `0` indicates success, error otherwise.
