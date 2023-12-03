<?php declare(strict_types=1);

class Cosine {
    public static function similarity(array $id1, array $id2): float {
        return self::dotp($id1, $id2) / sqrt(self::dotp($id1, $id1) * self::dotp($id2, $id2));
    }
    public static function dotp(array $arr1, array $arr2): float {
        return array_sum(array_map(fn($a, $b) => $a * $b, $arr1, $arr2));
    }
}
