<?php declare(strict_types=1);

use Kambo\Benchmark\AbstractBench;

use PhpBench\Attributes as Bench;

/**
 * @BeforeMethods({"init"})
 * @Iterations(10000)
 * @Revs(100)
 * @Warmup(100)
 * @OutputTimeUnit("microseconds", precision=5)
 */
class RustExtensionCallBench extends AbstractBench
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


    #[Bench\ParamProviders(['provideDataRust'])]
    public function benchSmallSize($params)
    {
        return cosine_similarity_rust($params['small'][0], $params['small'][1]);
    }

    #[Bench\ParamProviders(['provideDataRust'])]
    public function benchMiddleSize($params)
    {
        return cosine_similarity_rust($params['middle'][0], $params['middle'][1]);
    }

    #[Bench\ParamProviders(['provideDataRust'])]
    public function benchBigSize($params)
    {
        return cosine_similarity_rust($params['big'][0], $params['big'][1]);
    }
}
