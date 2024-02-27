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
class FFIPreparedCallBench extends AbstractBench
{
    private FFI $ffi;

    public function init()
    {
        $path = getcwd().DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR;
        require_once $path.DIRECTORY_SEPARATOR.'ffi_call.php';
        $this->ffi = FFI::cdef(
            "double cosine_similarity(const double *array1, const double *array2, int size);",
            __DIR__ . "/../../cosine_similarity_c_lib/libcosine_similarity.so"
        );
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchSmallSize($params)
    {
        return cosine_similarity_prepared_ffi($this->ffi, $params['small'][0], $params['small'][1]);
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchMiddleSize($params)
    {
        return cosine_similarity_prepared_ffi($this->ffi, $params['middle'][0], $params['middle'][1]);
    }

    #[Bench\ParamProviders(['provideData'])]
    public function benchBigSize($params)
    {
        return cosine_similarity_prepared_ffi($this->ffi, $params['big'][0], $params['big'][1]);
    }
}
