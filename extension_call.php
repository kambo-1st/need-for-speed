<?php

$start = microtime(true);
cosine_similarity([1,2,3,4,5], [3,4,5,6,7]);
$end = microtime(true) - $start;
echo 'Total Time (php-ext call): '.number_format($end * 1000000).'µs'.PHP_EOL;

