<?php declare(strict_types=1);

function cosine_similarity_alter(array $id1, array $id2): float {
    $dot_product_1_1 = 0.0;
    $dot_product_2_2 = 0.0;
    $dot_product_1_2 = 0.0;

    foreach ($id1 as $key => $value) {
        if (isset($id2[$key])) {
            $dot_product_1_2 += $value * $id2[$key];
        }
        $dot_product_1_1 += $value * $value;
    }

    foreach ($id2 as $value) {
        $dot_product_2_2 += $value * $value;
    }

    return $dot_product_1_2 / sqrt($dot_product_1_1 * $dot_product_2_2);
}

function cosine_similarity($array1, $array2) {
    $dot_product = 0.0;
    $magnitude_a = 0.0;
    $magnitude_b = 0.0;

    $size = count($array1);
    for ($i = 0; $i < $size; ++$i) {
        $dot_product += $array1[$i] * $array2[$i];
        $magnitude_a += $array1[$i] * $array1[$i];
        $magnitude_b += $array2[$i] * $array2[$i];
    }

    return $dot_product / (sqrt($magnitude_a) * sqrt($magnitude_b));
}

/*function cosine_similarity_unroll($array1, $array2) {
    $dot_product = 0.0;
    $magnitude_a = 0.0;
    $magnitude_b = 0.0;

    $size = count($array1);
    $i = 0;

    for (; $i < $size - 1; $i += 2) {
        $dot_product += $array1[$i] * $array2[$i] + $array1[$i + 1] * $array2[$i + 1];
        $magnitude_a += $array1[$i] * $array1[$i] + $array1[$i + 1] * $array1[$i + 1];
        $magnitude_b += $array2[$i] * $array2[$i] + $array2[$i + 1] * $array2[$i + 1];
    }

    if ($i < $size) {
        $dot_product += $array1[$i] * $array2[$i];
        $magnitude_a += $array1[$i] * $array1[$i];
        $magnitude_b += $array2[$i] * $array2[$i];
    }

    return $dot_product / (sqrt($magnitude_a) * sqrt($magnitude_b));
}*/

function cosine_similarity_unroll($array1, $array2) {
    $dot_product = 0.0;
    $magnitude_a = 0.0;
    $magnitude_b = 0.0;

    $size = count($array1);
    $i = 0;

    // Unrolled loop processing five elements per iteration
    for (; $i < $size - 4; $i += 5) {
        $dot_product += $array1[$i] * $array2[$i] + $array1[$i + 1] * $array2[$i + 1]
            + $array1[$i + 2] * $array2[$i + 2] + $array1[$i + 3] * $array2[$i + 3]
            + $array1[$i + 4] * $array2[$i + 4];
        $magnitude_a += $array1[$i] * $array1[$i] + $array1[$i + 1] * $array1[$i + 1]
            + $array1[$i + 2] * $array1[$i + 2] + $array1[$i + 3] * $array1[$i + 3]
            + $array1[$i + 4] * $array1[$i + 4];
        $magnitude_b += $array2[$i] * $array2[$i] + $array2[$i + 1] * $array2[$i + 1]
            + $array2[$i + 2] * $array2[$i + 2] + $array2[$i + 3] * $array2[$i + 3]
            + $array2[$i + 4] * $array2[$i + 4];
    }

    // Handle any remaining elements
    for (; $i < $size; ++$i) {
        $dot_product += $array1[$i] * $array2[$i];
        $magnitude_a += $array1[$i] * $array1[$i];
        $magnitude_b += $array2[$i] * $array2[$i];
    }

    return $dot_product / (sqrt($magnitude_a) * sqrt($magnitude_b));
}
