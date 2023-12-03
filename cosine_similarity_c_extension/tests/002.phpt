--TEST--
test1() Basic test
--EXTENSIONS--
cosine_similarity_c
--FILE--
<?php
var_dump(cosine_similarity_c([1,2,3,4,5], [3,4,5,6,7]));
?>
--EXPECT--
float(0.9864400504156211)