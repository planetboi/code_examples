function addPizza(id){
    "use strict"
    let pizza = document.getElementById(id);    
    let form = document.forms["orderForm"];
    let cart = form.elements["cart"];
    let newPizza = document.createElement("option");
    let text = pizza.getAttribute("data-name");
    let price = pizza.getAttribute("data-price");
    newPizza.text = text;
    newPizza.value = id;
    newPizza.selected = false;

    document.getElementById("cart").style.display = "block";
    cart.options[cart.length]=newPizza;

    addToPrice(price);
    
    
}

function addToPrice(price){

    "use strict";

    let element = document.getElementById("totalPrice");
    let currentPrice = element.getAttribute("data-totalPrice");
    let newPrice = parseFloat(price, 10) + parseFloat(currentPrice,10);
    element.setAttribute("data-totalPrice", newPrice);
    let total = newPrice.toFixed(2);
    element.textContent = total + "€";

}

function sendOrder(event){
    "use strict";


    let form = document.forms["orderForm"];
    let addresse = form.elements["address"];
    let cart = form.elements["cart"];
    if(cart.options.length < 1 || addresse.value.length == 0){
    event.preventDefault()
    }
    else{
        let pizzaSelected = false;
        for(let i=cart.length -1; i >= 0; i--){
            pizzaSelected += cart.options[i].selected;
    }
        if(!pizzaSelected){
            for(let i=cart.length -1; i >= 0; i--){
                cart.options[i].selected = true;
        }
        }
        
        form.submit();
    }
}

function deleteSelectedPizzas(event){
    "use strict";


    let form = document.forms["orderForm"];
    let cart = form.elements["cart"];
    for(let i=cart.length -1; i >= 0; i--){
        if(cart.options[i].selected == true){
            let id = cart.options[i].value;
            let pizza = document.getElementById(id);
            let price = parseFloat(pizza.getAttribute("data-price")) * -1;
            addToPrice(price);
            cart.remove(i);
        }
    }

    if(cart.length < 1){
        document.getElementById("cart").style.display = "none";
    }

    event.preventDefault();

}

function deleteAllPizzas(event){
    "use strict";


    let form = document.forms["orderForm"];
    for(let i=cart.length -1; i >= 0; i--){
            cart.remove(i);
    }

    document.getElementById("cart").style.display = "none";
    let element = document.getElementById("totalPrice");
    let currentPrice = parseFloat(element.getAttribute("data-totalPrice"),10) * - 1;
    addToPrice(currentPrice);

    event.preventDefault();

}

function automaticSubmit(formid){
    "use strict";

    let form = document.forms[formid];
    form.submit();
}