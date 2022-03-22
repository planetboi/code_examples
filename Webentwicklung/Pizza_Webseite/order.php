<?php

declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

require_once './Page.php';

class Pizza
{
    public string $id;
    public string $name;
    public string $price;
    public string $image;
    function __construct(string $id, string $name, string $price, string $image)
    {
        $this->id = $id;
        $this->name = $name;
        $this->price = $price;
        $this->image = $image;
    }
}

class Order extends Page
{
    // to do: declare reference variables for members 
    // representing substructures/blocks

    /**
     * Instantiates members (to be defined above).
     * Calls the constructor of the parent i.e. page class.
     * So, the database connection is established.
     * @throws Exception
     */
    protected function __construct()
    {
        parent::__construct();
        // to do: instantiate members representing substructures/blocks
    }

    /**
     * Cleans up whatever is needed.
     * Calls the destructor of the parent i.e. page class.
     * So, the database connection is closed.
     */
    public function __destruct()
    {
        parent::__destruct();
    }

    /**
     * Fetch all data that is necessary for later output.
     * Data is returned in an array e.g. as associative array.
     * @return array An array containing the requested data. 
     * This may be a normal array, an empty array or an associative array.
     */
    protected function getViewData(): array
    {
        $sql = "SELECT `article_id`, `name`, `picture`, `price` FROM article ORDER BY `article_id`";

        $recordset = $this->_database->query($sql);
        if (!$recordset) {
            throw new Exception("Abfrage fehlgeschlagen: " . $this->_database->error);
        }

        $pizzas = array();
        $data = array();
        while ($record = $recordset->fetch_assoc()) {
            $data["article_id"] = $record["article_id"];
            $data["name"] = $record["name"];
            $data["picture"] = $record["picture"];
            $data["price"] = $record["price"];
            $pizzas[$record["article_id"]] = $data; // Array of arrays
        }
        $recordset->free();
        return $pizzas;
    }


    /**
     * First the required data is fetched and then the HTML is
     * assembled for output. i.e. the header is generated, the content
     * of the page ("view") is inserted and -if available- the content of
     * all views contained is generated.
     * Finally, the footer is added.
     * @return void
     */
    protected function generateView(): void
    {
        $viewData = $this->getViewData(); // NOSONAR ignore unused $data
        $this->generatePageHeader('Bestellung');

        echo <<<HTML
        <!--SPEISEKARTE MIT VERSCHIEDENEN PIZZEN-->
        <section>
            <h2 id="menuTitle">Speisekarte</h2>
            <div class="menu">
HTML;
$counter = 0;


        foreach ($viewData as $pizzas => $data) {
            $id = $data["article_id"];
            $name = $data["name"];
            $picture = $data["picture"];
            $price = $data["price"];

            $pizza = new Pizza($id, $name, $price, $picture);
            $_name = htmlspecialchars($pizza->name);
            $_picture = htmlspecialchars($pizza->image);
            $_price = htmlspecialchars($pizza->price);
            
            
            echo <<<HTML
            <article id={$id} data-name={$_name} data-price={$_price}>
            <h3>$_name</h3>
            <img src={$_picture} alt="" onclick="addPizza({$id})"><br/>
           <p class="priceText">{$_price}€</p>
        </article>
        HTML;
        $counter += 1;
        }

        echo <<<EOD
        </div>
        </section>
            <!--WARENKORB-->
        <form name="orderForm" action="order.php" method="post" accept-charset="UTF-8">
        <label id="cartLabel">Warenkorb
            <select id="cart" name="cart[]" tabindex="0" multiple>
        </select>
        </label>
        <!--Price (generated from database)-->
        <p>Gesamtpreis:  <span id="totalPrice" data-totalPrice=0.00>0.00€</span></p>

        <!--Addresse-->
        <label>Lieferadresse
            <input type="text" id="address" name="address" value="" placeholder="Ihre Adresse" required/>
        </label>
        <!--Buttons-->
        <p>
        <button id="deleteAll" value="Alle löschen" onclick="deleteAllPizzas(event)">Alle löschen</button>
        <button id="deleteChoice" value="Auswahl löschen" onclick="deleteSelectedPizzas(event)">Auswahl löschen</button>
        <button type="submit" id="submitOrder" value="Bestellen" onclick="sendOrder(event)">Bestellen</button>
        </p>
        </form>
        
        EOD;
        $this->generatePageFooter();
    }

    /**
     * Processes the data that comes via GET or POST.
     * If this page is supposed to do something with submitted
     * data do it here.
     * @return void
     */
    protected function processReceivedData(): void
    {
        parent::processReceivedData();
        session_start();

        // Weiterleitung nach PRG-Pattern
        if (count($_POST)){

        // Check if post data was received
        if (!isset($_POST)) return;

        // Check if address was received
        if (!isset($_POST["address"])) return; 

        
        $address = $this->_database->real_escape_string($_POST["address"]);

        $sql = "INSERT INTO `ordering` (address) VALUES ('$address')";
        $result = $this->_database->query($sql);

        // Check if we got a query back
        if (!$result) {
            echo "<h1>Ups!</h1>Ein Fehler ist aufgetreten. Keine Angst, das ist unsere Schuld.";
            return;
        }
        
        
        $orderID = $this->_database->insert_id;
        //  SAVE ORDERID IN SESSION
        $_SESSION['OrderID'] = $orderID;
        

        foreach ($_POST as $key => $value) {

            // Proceed with next element if $value is not an array
            if (!is_array($value)) continue;

            foreach ($value as $number => $articleID) {

                //Injection protection
                if (!is_numeric($articleID)) continue;

                $sql = "INSERT INTO `ordered_article` (ordering_id, article_id) VALUES ((SELECT ordering_id from `ordering` WHERE ordering_id='$orderID'),(SELECT article_id from `article` WHERE article_id='$articleID'))";

                if (!$this->_database->query($sql)) {
                    throw new Exception("Insert fehlgeschlagen: " . $this->_database->error);
                }
            
            }
        }
    
        header("HTTP/1.1 303 See Other");
        header("Location: " . "order.php");
        die;
    }
        
    }

    /**
     * This main-function has the only purpose to create an instance
     * of the class and to get all the things going.
     * I.e. the operations of the class are called to produce
     * the output of the HTML-file.
     * The name "main" is no keyword for php. It is just used to
     * indicate that function as the central starting point.
     * To make it simpler this is a static function. That is you can simply
     * call it without first creating an instance of the class.
     * @return void
     */
    public static function main(): void
    {
        try {
            $page = new Order();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            //header("Content-type: text/plain; charset=UTF-8");
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

// This call is starting the creation of the page. 
// That is input is processed and output is created.
Order::main();
