//VON GRUPPENMITGLIED
function process(json_string) {
    "use strict";
    let data = JSON.parse(json_string)
    console.log(data)

    let node = document.getElementById("orders");
    node.innerHTML = '';

    for(let entry of data) {
        let status;
        switch (entry.status) {
            case "0":
                status = "Bestellt"
                break;
            case "1":
                status = "Im Ofen"
                break;
            case "2":
                status = "Fertig gebacken"
                break;
            case "3":
                status = "Unterwegs"
                break;
            case "4":
                // This should never ever happen
                status = "Geliefert"
                break;
            default:
                // This should happen even less
                status = "Unbekannt"
                break;
        }
        let paragraph = document.createElement("p")
        node.appendChild(paragraph)
        let text = `${entry.name}: ${status}`
        let textNode = document.createTextNode(text)
        paragraph.appendChild(textNode)
    }
    console.log("Status updated")
}

let request= new XMLHttpRequest();

function processData(){
    "use strict";
    if(request.readyState !== 4) { return }
    // Full request received
    if(request.status !== 200) {
        console.error("Übertragung fehlgeschlagen")
        return
    }
    // HTTP Statuscode OK
    if(request.responseText == null) {
        console.error("Dokument ist leer")
        return
    }

    process(request.responseText)
}

function requestData() {
    "use strict";
    request.open("GET", "KundenStatus.php");
    request.onreadystatechange = processData;// Callback-Handler zuordnen
    request.send();
}

function startAutoRefresh() {
    "use strict";
    // Call once upon page load
    requestData()

    // Register continuous calls
    window.setInterval (()=>requestData(), 2000)
}
