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
        $result = cosine_similarity_rust([1,2,3,4,5], [3,4,5,6,7]);

        $this->assertEqualsWithDelta(0.9864400504156211, $result, 0.0001);
    }

    public function testMiddleSize()
    {
        $result = cosine_similarity_rust(
            [
                15726,
                38758,
                717,
                34061,
                29829,
                32705,
                37367,
                41401,
                43317,
                52172,
                11350,
                61375,
                20052,
                54100,
                43091,
            ],
            [
                22233,
                11520,
                37106,
                15471,
                42414,
                42536,
                27226,
                60988,
                42317,
                42320,
                63733,
                63733,
                63733,
                63733,
                63733,
            ]
        );
        $this->assertEqualsWithDelta(0.8706110919011579, $result, 0.0001);
    }

    public function testBigSize()
    {
        $result = cosine_similarity_rust(
            [
                15726,
                38758,
                717,
                34061,
                29829,
                32705,
                37367,
                41401,
                43317,
                52172,
                11350,
                61375,
                20052,
                54100,
                43091,
                22233,
                11520,
                37106,
                15471,
                42414,
                42536,
                27226,
                60988,
                42317,
                42320,
                63733,
                63733,
                63733,
                63733,
                63733,
            ],
            [
                15726,
                38758,
                717,
                34061,
                29829,
                32705,
                37367,
                41401,
                43317,
                52172,
                11350,
                61375,
                20052,
                54100,
                43091,
                22233,
                11520,
                37106,
                15471,
                42414,
                42536,
                27226,
                60988,
                42317,
                42320,
                63733,
                63733,
                63733,
                63733,
                63733,
            ]
        );
        $this->assertEqualsWithDelta(1.0, $result, 0.0001);
    }
}
