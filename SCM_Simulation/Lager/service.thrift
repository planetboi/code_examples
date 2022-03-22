namespace cpp Service

struct Product {

1: string name,
2: i32 id,
3: i32 amount

}

service Service
{

list<Product> seeProducts(1:string warehouse),
string orderProduct(1:string product, 2:i32 amount, 3:string warehouse)

}