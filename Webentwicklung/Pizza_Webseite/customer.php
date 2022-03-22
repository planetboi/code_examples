<?php declare(strict_types=1);
//VON GRUPPENMITGLIED
// UTF-8 marker äöüÄÖÜß€

require_once './Page.php';

class Item
        {
            public string $name;
            public int $status = 0;
    
            function __construct(string $name, int $status){
                $this->name=$name;
                $this->status=$status;
            }
}

class Customer extends Page
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
    protected function getViewData():array
    {}

    /**
     * First the required data is fetched and then the HTML is
     * assembled for output. i.e. the header is generated, the content
     * of the page ("view") is inserted and -if available- the content of
     * all views contained is generated.
     * Finally, the footer is added.
	 * @return void
     */
    protected function generateView():void
    {





        $debug = htmlspecialchars('[{"ordered_article_id":"59","name":"Salami","status":"0"},{"ordered_article_id":"60","name":"Vegetaria","status":"0"}]');
        $title = "Ihr Bestellstatus";

        header("Content-type: text/html; charset=UTF-8");
        echo <<<EOT
        <!DOCTYPE html>
        <html lang="de">
        <head>
          <meta charset="UTF-8" />
          <link rel="stylesheet" type="text/css"href="styles.css">
          <title>$title</title>
        </head>
        <body onload="startAutoRefresh()">
        <script src="StatusUpdate.js"></script>
        <header></header>
        <h1>$title</h1>
        <div id="orders"></div>
        <input type="button" onClick="location.href='order.php'" value="Neue Bestellung"/>
        EOT;


        $this->generatePageFooter();
    }

    /**
     * Processes the data that comes via GET or POST.
     * If this page is supposed to do something with submitted
     * data do it here.
	 * @return void
     */
    protected function processReceivedData():void
    {
        parent::processReceivedData();
        // to do: call processReceivedData() for all members
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
    public static function main():void
    {
        try {
            $page = new Customer();
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
Customer::main();