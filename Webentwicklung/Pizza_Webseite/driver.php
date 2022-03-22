<?php declare(strict_types=1);
//VON GRUPPENMITGLIED
// UTF-8 marker äöüÄÖÜß€

require_once './Page.php';

class Customer
        {
            public int $id;
            public string $address;
            public string $order;
            public int $status;

            function __construct(int $id, string $address, string $order){
                $this->id=$id;
                $this->address=$address;
                $this->order=$order;
                $this->status = 0;
            }
}

class Driver extends Page
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
        // ID beinhaltet implizit die Anzeigereihenfolge
        $sql = <<<EOD
SELECT * FROM (
    SELECT
        ordering.ordering_id,
        address,
        MIN(STATUS) AS ordering_status,
        GROUP_CONCAT(NAME) AS pizzas,
        ROUND(SUM(price), 2) AS total
    FROM ordering
    INNER JOIN ordered_article
        ON ordering.ordering_id = ordered_article.ordering_id
    INNER JOIN article
        ON ordered_article.article_id = article.article_id
    GROUP BY
        ordering.ordering_id
)
    AS t2
    WHERE ordering_status
        BETWEEN 2 AND 3
;
EOD;
        
        $recordset = $this->_database->query($sql);
        if (!$recordset) {
            throw new Exception("Abfrage fehlgeschlagen: " . $this->_database->error);
        }

        $elemente = array();
        $data = array();
        while ($record = $recordset->fetch_assoc()) {
            $data["address"] = $record["address"];
            $data["ordering_status"] = $record["ordering_status"];
            $data["pizzas"] = $record["pizzas"];
            $data["total"] = $record["total"];
            $elemente[$record["ordering_id"]] = $data; // Array of arrays
        }
        $recordset->free();
        return $elemente;
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
		$viewData = $this->getViewData(); // NOSONAR ignore unused $data

        $this->generatePageHeader('Fahrer*in');
        echo <<<EOD
  <form action="driver.php" id="delivery_form" method="post" accept-charset="UTF-8">
EOD;
        
        foreach($viewData as $key => $data) {
            $address = htmlspecialchars($data["address"]);
            $total = $data["total"];
            $pizzas = htmlspecialchars($data["pizzas"]);
            $check2 = $data["ordering_status"] == 2 ? "checked" : "";
            $check3 = $data["ordering_status"] == 3 ? "checked" : "";

            echo <<<EOD
    <fieldset>
      <legend>${address} - {$total} - {$pizzas}</legend>
      <label>Fertig
        <input type="radio" name="{$key}" value="2" onclick="automaticSubmit('delivery_form')" ${check2}>
      </label>
      <label>Unterwegs
        <input type="radio" name="{$key}" value="3" onclick="automaticSubmit('delivery_form')" ${check3}>
      </label>
      <label>Geliefert
        <input type="radio" name="{$key}" value="4" onclick="automaticSubmit('delivery_form')">
      </label>
    </fieldset>
    
    EOD;
        }

        echo <<<EOD
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
    protected function processReceivedData():void
    {
        parent::processReceivedData();

        if(count($_POST)) {
            // Datenverarbeitung
            if (isset($_POST)) {
                foreach ($_POST as $key => $value) {

                    //Injection protection
                    if(!is_numeric($value)) {
                        throw new Exception("Not a valid status.");
                    }
                    if(!is_numeric($key)) {
                        throw new Exception("Not a valid id.");
                    }

                    $sql = "UPDATE ordered_article SET status = {$value} WHERE ordering_id = {$key};";

                    if (!$this->_database->query($sql)) {
                        throw new Exception("Insert fehlgeschlagen: " . $this->_database->error);
                    }

                }
            }

            // Weiterleitung nach PRG-Pattern
            header("HTTP/1.1 303 See Other");
            header("Location: ". "driver.php");
            die();
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
    public static function main():void
    {
        try {
            $page = new Driver();
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
Driver::main();