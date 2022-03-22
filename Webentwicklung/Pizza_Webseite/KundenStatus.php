<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

require_once './Page.php';

class KundenStatus extends Page
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
    {
        session_start();

        $condition = "";
        if(isset($_SESSION['OrderID'])) {
            $orderID = $_SESSION['OrderID'];
            if(is_numeric($orderID)){
                $condition = "WHERE ordering_id = {$orderID} ";
            }
        }
        $sql = "SELECT  `ordered_article_id`, `name`, `status` FROM ordered_article INNER JOIN article on ordered_article.article_id = article.article_id {$condition} ORDER BY `ordered_article_id`";

        $recordset = $this->_database->query($sql);
        if(!$recordset){throw new Exception("Abfrage fehlgeschlagen: " . $this->_database->error);}

        $items = array();
        $data = array();
        while ($record = $recordset->fetch_assoc()) {
            $data["ordered_article_id"] = $record["ordered_article_id"];
            $data["name"] = $record["name"];
            $data["status"] = $record["status"];
            $items[] = $data;
        }
        $recordset->free();
        return $items;
    }

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
        header("Content-type: application/json; charset=UTF-8");

        $viewData = $this->getViewData();
        
        echo json_encode($viewData);

    }

    /**
     * Processes the data that comes via GET or POST.
     * If this page is supposed to do something with submitted
     * data do it here.
	 * @return void
     */
    protected function processReceivedData():void {}

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
        header("Cache-Control: no-store, no-cache, must-revalidate"); // HTTP/1.1
        header("Expires: Sat, 01 Jul 2000 06:00:00 GMT"); // Datum in der Vergangenheit
        header("Cache-Control: post-check=0, pre-check=0", false); // fuer IE
        header("Pragma: no-cache");
        session_cache_limiter('nocache'); // VOR session_start()!
        session_cache_expire(0);

        try {
            $page = new KundenStatus();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

// This call is starting the creation of the page. 
// That is input is processed and output is created.
KundenStatus::main();
