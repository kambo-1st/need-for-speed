use ext_php_rs::prelude::*;
use ext_php_rs::types::Zval;

#[php_function]
pub fn cosine_similarity(array1: Vec<f64>, array2: Vec<f64>) -> Result<Zval, String> {
    let mut dot_product = 0.0;
    let mut magnitude_a = 0.0;
    let mut magnitude_b = 0.0;

    for (x, y) in array1.iter().zip(array2.iter()) {
        dot_product += x * y;
        magnitude_a += x.powi(2);
        magnitude_b += y.powi(2);
    }

    let result = dot_product / (magnitude_a.sqrt() * magnitude_b.sqrt());

    let mut zval_result = Zval::new();
    zval_result.set_double(result);
    Ok(zval_result)
}

#[php_module]
pub fn get_module(module: ModuleBuilder) -> ModuleBuilder {
    module
}

