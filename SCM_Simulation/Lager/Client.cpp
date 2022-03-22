#include "Client.h"

void Client::addStock(std::string & _return, const std::string & product, const int32_t amount, const std::string & warehouse)
    {

		string clientID;
char* clientChar = getenv ("LAGERID");
        if (clientChar==nullptr) {
            printf ("No client ID");
            throw runtime_error ("MQTT Pub Error: Client ID missing");
        }

clientID = clientChar;

        size_t found = _return.find("Error");

        if (found != string::npos)
        {
            cerr << "Error: order was not placed." << endl;
        }

        else
        {
			int old = lager->countStock(product);
            lager->changeStock(product, amount);
			lager->saveStock("");
			stringstream msg;
                        msg << amount << " pieces of " << product << " was added" << " from " << warehouse << " to " << clientID << ". Stock was updated from " << old << " to " <<lager->countStock(product) << endl;
                        cout << msg.str();

            }

    }

void Client::rpc(){
 
	try
	{

        //get name of server to get ip address
        char* rpclager = getenv ("RPCLAGER");
        if (rpclager==nullptr) {
            printf ("No server name sent");
            throw runtime_error ("Sensor Error: Lager name missing");
        }

        string s(rpclager);
        //cout << s << endl;
         //get port
        char* clientPortStr = getenv ("CLIENTPORT");
        if (clientPortStr==nullptr) {
            printf ("No port number sent");
            throw runtime_error ("Sensor Error: Port number missing");
        }
        int clientPort = atoi(clientPortStr);

std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    auto socket = make_shared<TSocket>(s, clientPort);
	auto transport = make_shared<TBufferedTransport>(socket);
	auto protocol = make_shared<TBinaryProtocol>(transport);


	ServiceClient client(protocol);

        cout << "[RPC Client] Online! Ich werde regelmäßig den Produktbestand eines anderen Lagers abfragen und Bestellungen und Lieferungen veranlassen :)" << endl;


        while(true){

            if(*endRun){
                cout << "RPC thread ending" << endl;
                return;
            }
            transport->open();

		vector<Product> result;

		client.seeProducts(result, s);


		size_t i = 0;
        stringstream msg;
        cout << endl;
		while(i < result.size()){
		    msg << "Product: " << result.at(i).name << "\tID: " << result.at(i).id << "\tAmount: " << result.at(i).amount << endl;
		    i++;

            
		}
        cout << msg.str() << endl << endl;

transport->close();

std::this_thread::sleep_for(std::chrono::milliseconds(5000));

if(*endRun){
                cout << "RPC thread ending" << endl;
                return;
            }
            
            transport->open();
		
		string orderStatus;
        
        int randNumber;
        int randAmount;
        string randProduct;

        srand((unsigned) time(0)*clientPort);
        randAmount = (rand() % 10) + 1;
        randNumber = (rand() % 10) + 1;

        switch (randNumber) {
        case 1:
            randProduct = "Schuhe";
            break;
        case 2:
            randProduct = "Hose";
            break;
        case 3:
            randProduct = "Jeans";
            break;
        case 4:
            randProduct = "Shirt";
            break;
        case 5:
            randProduct = "Pullover";
            break;
        case 6:
            randProduct = "Hemd";
            break;
        case 7:
            randProduct = "Kleid";
            break;
        case 8:
            randProduct = "Rock";
            break;
        case 9:
            randProduct = "Jacke";
            break;
        case 10:
            randProduct = "Mantel";
            break;
        default:
            randProduct ="";
            break;
    }
        
        client.orderProduct(orderStatus, randProduct, randAmount, s);
        cout << "Status: " << orderStatus << endl;
        addStock(orderStatus, randProduct, randAmount, s);
        
 transport->close();
         std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        }
		
	}
	catch(TException& tx)
	{
		cout << "ERROR: " << tx.what() << endl;
	}

}