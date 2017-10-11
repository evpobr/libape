# ape_file_get_compression_level

Gets compression level of APE file.

## Syntax

```c
APE_EXPORT int ape_file_get_compression_level(ape_file *ctx);
```

### Parameters

**in** `ctx`  
APE context.

## Remarks

Supported compression levels are `APE_COMPRESSION_LEVEL_FAST`, `APE_COMPRESSION_LEVEL_NORMAL`, `APE_COMPRESSION_LEVEL_HIGH`, `APE_COMPRESSION_LEVEL_EXTRA_HIGH` and `APE_COMPRESSION_LEVEL_INSANE`.

## Return value

Return value >= `0` indicates success, error otherwise.

## See also

[`APE_COMPRESSION_LEVEL_FAST`](ape_compression_level_fast.md), [`APE_COMPRESSION_LEVEL_NORMAL`](ape_compression_level_normal.md), [`APE_COMPRESSION_LEVEL_HIGH`](ape_compression_level_high.md), [`APE_COMPRESSION_LEVEL_EXTRA_HIGH`](ape_compression_level_extra_high.md), [`APE_COMPRESSION_LEVEL_INSANE`](ape_compression_level_insane.md)
