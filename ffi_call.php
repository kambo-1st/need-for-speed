<?php declare(strict_types=1);

function cosine_similarity_ffi(array $a, array $b) : float {
    $fii = FFI::cdef(
        "double cosine_similarity(const double *array1, const double *array2, int size);",
        __DIR__ . "/cosine_similarity_c_lib/libcosine_similarity.so"
    );

    $size = count($a);
    $aNative = $fii->new("double[$size]", false);
    $bNative = $fii->new("double[$size]", false);

    foreach ($a as $key => $value) {
        $aNative[$key] = $value;
    }

    foreach ($b as $key => $value) {
        $bNative[$key] = $value;
    }

    return $fii->cosine_similarity($aNative, $bNative, $size);
}

function cosine_similarity_prepared_ffi(FFI $fii, array $a, array $b) : float {
    $size = count($a);
    $aNative = $fii->new("double[$size]", false);
    $bNative = $fii->new("double[$size]", false);

    foreach ($a as $key => $value) {
        $aNative[$key] = $value;
    }

    foreach ($b as $key => $value) {
        $bNative[$key] = $value;
    }

    return $fii->cosine_similarity($aNative, $bNative, $size);
}
