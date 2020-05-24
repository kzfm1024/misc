fn main() {
    // let x = vec![1, 2, 3];
    // let equal_to_x = move |z| z == x;
    // let y = vec![1, 2, 3];
    // assert!(equal_to_x(y));

    let v1 = vec![1, 2, 3];
    let v1_iter = v1.iter();
    for val in v1_iter {
        println!("Got: {}", val);
    }
    
    let v2 : Vec<i32> = v1.iter().map(|x| x + 1).collect();
    assert_eq!(v2, vec![2, 3, 4]); 
    println!("v2: {:?}", v2);
    // println!("v2.collect(): {:?}", v2.collect());
}

#[test]
fn iterator_demonstration() {
    let v1 = vec![1, 2, 3];
    let mut v1_iter = v1.iter();
    assert_eq!(v1_iter.next(), Some(&1));
    assert_eq!(v1_iter.next(), Some(&2));
    assert_eq!(v1_iter.next(), Some(&3));
    assert_eq!(v1_iter.next(), None);
}

#[test]
fn iterator_sum() {
    let v1 = vec![1, 2, 3];
    let v1_iter = v1.iter();
    let total: i32 = v1_iter.sum();
    assert_eq!(total, 6);
}

// #[test]
// fn iterator_map() {
//     let v1 = vec![1, 2, 3];
//     let v2 = vec![2, 3, 4];    

//     let v1_iter = v1.iter();
//     assert_eq!(v1.iter().map(|x| x + 1).collect(), v2);
// }

