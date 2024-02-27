<?php declare(strict_types=1);

namespace Kambo\Benchmark\Tests;

use PHPUnit\Framework\TestCase;
use Cosine;

class RustExtensionTest extends TestCase
{
    /**
     * @beforeClass
     */
    public static function loadExtension(): void
    {
        $result = dl('libcosine_similarity.so');
        if ($result === false) {
            $error = 'Unable to load libcosine_similarity.so';
            if (!file_exists(ini_get('extension_dir').'/libcosine_similarity.so')) {
                $error = 'Extension in '.ini_get('extension_dir').'/libcosine_similarity.so not found.';
                $error .= PHP_EOL.'Please run `make install` in the extension directory';
            }

            throw new \RuntimeException($error);
        }
    }

    public function testSmallSize()
    {
        $result = cosine_similarity_rust([1.0,2.0,3.0,4.0,5.0], [3.0,4.0,5.0,6.0,7.0]);

        $this->assertEqualsWithDelta(0.9864400504156211, $result, 0.0001);
    }

    public function testMiddleSize()
    {
        $result = cosine_similarity_rust(
            [
                15726.0,
                38758.0,
                717.0,
                34061.0,
                29829.0,
                32705.0,
                37367.0,
                41401.0,
                43317.0,
                52172.0,
                11350.0,
                61375.0,
                20052.0,
                54100.0,
                43091.0,
            ],
            [
                22233.0,
                11520.0,
                37106.0,
                15471.0,
                42414.0,
                42536.0,
                27226.0,
                60988.0,
                42317.0,
                42320.0,
                63733.0,
                63733.0,
                63733.0,
                63733.0,
                63733.0,
            ]
        );
        $this->assertEqualsWithDelta(0.8706110919011579, $result, 0.0001);
    }

    public function testBigSize()
    {
        $result = cosine_similarity_rust(
            [
                15726.0,
                38758.0,
                717.0,
                34061.0,
                29829.0,
                32705.0,
                37367.0,
                41401.0,
                43317.0,
                52172.0,
                11350.0,
                61375.0,
                20052.0,
                54100.0,
                43091.0,
                22233.0,
                11520.0,
                37106.0,
                15471.0,
                42414.0,
                42536.0,
                27226.0,
                60988.0,
                42317.0,
                42320.0,
                63733.0,
                63733.0,
                63733.0,
                63733.0,
                63733.0,
            ],
            [
                15726.0,
                38758.0,
                717.0,
                34061.0,
                29829.0,
                32705.0,
                37367.0,
                41401.0,
                43317.0,
                52172.0,
                11350.0,
                61375.0,
                20052.0,
                54100.0,
                43091.0,
                22233.0,
                11520.0,
                37106.0,
                15471.0,
                42414.0,
                42536.0,
                27226.0,
                60988.0,
                42317.0,
                42320.0,
                63733.0,
                63733.0,
                63733.0,
                63733.0,
                63733.0,
            ]
        );
        $this->assertEqualsWithDelta(1.0, $result, 0.0001);
    }
}
