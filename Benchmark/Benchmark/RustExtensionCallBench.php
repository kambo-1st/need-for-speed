<?php declare(strict_types=1);

/**
 * @BeforeMethods({"init"})
 * @Iterations(100)
 * @Revs(100)
 * @Warmup(100)
 * @OutputTimeUnit("microseconds", precision=5)
 */
class RustExtensionCallBench
{
    public function init()
    {
        /*$path = getcwd().DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR;
        require_once $path.DIRECTORY_SEPARATOR.'function_call.php';*/

        $result = dl('libcosine_similarity.so');
        if ($result === false) {
            $error = 'Unable to load cosine_similarity_c.so';
            if (!file_exists(ini_get('extension_dir').'/libcosine_similarity.so')) {
                $error = 'Extension in '.ini_get('extension_dir').'/libcosine_similarity.so not found.';
                $error .= PHP_EOL.'Please run `make install` in the extension directory';
            }

            throw new \RuntimeException($error);
        }
    }


    /**
     *
     */
    public function benchSmallSize()
    {
        cosine_similarity_rust([1,2,3,4,5], [3,4,5,6,7]);
    }

    /**
     *
     */
    public function benchMiddleSize()
    {
        cosine_similarity_rust(
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
                40254,
                63997,
                9055,
                48632,
            ],
        );
    }

    /**
     *
     */
    public function benchBigSize()
    {
        cosine_similarity_rust(
            [
                3531,
                10535,
                39078,
                19332,
                33455,
                17548,
                9355,
                3538,
                3514,
                35074,
                30735,
                57587,
                63812,
                60721,
                18105,
                60242,
                60326,
                50274,
                8189,
                61505,
                42904,
                2807,
                5964,
                16608,
                41958,
                35392,
                60922,
                24519,
                21045,
                23038,
                60056,
                19333,
                1200,
                32938,
                64379,
            ],
            [
                55470,
                877,
                62846,
                27251,
                20439,
                21041,
                3744,
                50679,
                56094,
                61527,
                34111,
                62907,
                10463,
                5505,
                12278,
                19729,
                43930,
                56496,
                2293,
                36249,
                15930,
                10333,
                20314,
                58318,
                14509,
                8465,
                43225,
                11753,
                30193,
                31740,
                49624,
                52317,
                2153,
                28334,
                58630,
            ],
        );
    }
}
