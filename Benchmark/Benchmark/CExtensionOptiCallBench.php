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
class CExtensionOptiCallBench extends AbstractBench
{
    public function init()
    {
        /*$path = getcwd().DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR;
        require_once $path.DIRECTORY_SEPARATOR.'function_call.php';*/

        $result = dl('cosine_similarity_c.so');
        if ($result === false) {
            $error = 'Unable to load cosine_similarity_c.so';
            if (!file_exists(ini_get('extension_dir').'/cosine_similarity_c.so')) {
                $error = 'Extension in '.ini_get('extension_dir').'/cosine_similarity_c.so not found.';
                $error .= PHP_EOL.'Please run `make install` in the extension directory';
            }

            throw new \RuntimeException($error);
        }
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchSmallSize($params)
    {
        return cosine_similarity_c_opti($params['small'][0], $params['small'][1]);
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchMiddleSize($params)
    {
        return cosine_similarity_c_opti($params['middle'][0], $params['middle'][1]);
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchBigSize($params)
    {
        return cosine_similarity_c_opti($params['big'][0], $params['big'][1]);
    }
}
