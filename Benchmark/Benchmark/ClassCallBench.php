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
class ClassCallBench extends AbstractBench
{
    public function init()
    {
        $path = getcwd().DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR;
        require_once $path.DIRECTORY_SEPARATOR.'class_call.php';
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchSmallSize($params)
    {
        return Cosine::similarity($params['small'][0], $params['small'][1]);
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchMiddleSize($params)
    {
        return Cosine::similarity($params['middle'][0], $params['middle'][1]);
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchBigSize($params)
    {
        return Cosine::similarity($params['big'][0], $params['big'][1]);
    }
}
