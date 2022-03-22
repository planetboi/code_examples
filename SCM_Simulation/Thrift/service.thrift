namespace cpp Service

struct Product {

1: string name,
2: i32 id,
3: i32 amount

}

service Service
{

list<Product> seeProducts(),
string orderProduct(1:string product, 2:i32 amount)

}