<?php

declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

require_once './Page.php';

class Baker extends Page
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
        // ID beinhaltet implizit die Anzeigereihenfolge
        $sql = "SELECT name, ordered_article_id, status, ordering_id FROM ordered_article INNER JOIN article ORDER BY ordered_article_id";

        $recordset = $this->_database->query($sql);
        if (!$recordset) {
            throw new Exception("Abfrage fehlgeschlagen: " . $this->_database->error);
        }

        $elemente = array();
        $data = array();
        while ($record = $recordset->fetch_assoc()) {
            $data["name"] = $record["name"];
            $data["status"] = $record["status"];
            $data["ordering_id"] = $record["ordering_id"];
            $elemente[$record["ordered_article_id"]] = $data; // Array of arrays
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
    protected function generateView(): void
    {
        //$data = $this->getViewData(); // NOSONAR ignore unused $data
        $viewData = $this->getViewData();
        $this->generatePageHeader('Bäcker*in');
        echo <<<EOT
  <form action="baker.php" id="pizza_form" method="post" accept-charset="UTF-8">
EOT;

        function pizzaStatus(string $ordering_id, $name, $ordered_article_id, $status)
        {

            $status0 = $status == 0 ? "checked" : "";
            $status1 = $status == 1 ? "checked" : "";
            $status2 = $status == 2 ? "checked" : "";

            echo <<<EOT

    <fieldset class="bakerStatus">
      <legend>Bestellung {$ordering_id}: Pizza {$name}</legend>
      <label>Bestellt
        <input type="radio" name="{$ordered_article_id}" value="0" onclick="automaticSubmit('pizza_form')" {$status0}>
      </label>
      <label>Im Ofen
        <input type="radio" name="{$ordered_article_id}" value="1" onclick="automaticSubmit('pizza_form')" {$status1}>
      </label>
      <label>Fertig
        <input type="radio" name="{$ordered_article_id}" value="2" onclick="automaticSubmit('pizza_form')" {$status2}>
      </label>
    </fieldset>

EOT;
        }

        foreach ($viewData as $key => $data) {
            if ($data["status"] < 3) {
                pizzaStatus($data["ordering_id"], htmlspecialchars($data["name"]), $key, $data["status"]);
            }
        }

        echo <<<EOT
  </form>
EOT;

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

        if (count($_POST)) {
            if (isset($_POST)) {
                // Datenverarbeitung
                foreach ($_POST as $key => $value) {

                    //Injection protection
                    if (!is_numeric($value)) {
                        throw new Exception("Not a valid status.");
                    }
                    if (!is_numeric($key)) {
                        throw new Exception("Not a valid id.");
                    }

                    $sql = "UPDATE ordered_article SET status = {$value} WHERE ordered_article_id = {$key};";

                    if (!$this->_database->query($sql)) {
                        throw new Exception("Insert fehlgeschlagen: " . $this->_database->error);
                    }
                }
            }

            // Weiterleitung nach PRG-Pattern
            header("HTTP/1.1 303 See Other");
            header("Location: " . "baker.php");
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
    public static function main(): void
    {
        try {
            $page = new Baker();
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
Baker::main();
