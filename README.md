# Beschreibung

Dieses Testbeispiel ist im Zuge einer Ausarbeitung entstanden.

- "asionvs"
	- Beinhaltet das Beispiel

- "asyncnetprog.pptx"
	- eine Präsentation über einige Konzepte der ASIO 

# Funktionsweise

## messages des clients:
	"join <NAME>"
	"shalom"
	"ask_for_clients"
	


## messages des servers:
	"join ok"
	"shalom client_constellation_changed"
        "shalom ok"
	"clients <MOMENTAN VERBUNDENE CLIENTS>"



# Kurzbeschreibung des Protokolls

client versucht dem server mit "join" zu joinen
server antwortet darauf mit "join ok". 

der client erfragt dann 

client schickt danach in einem zufälligen zeitabstand ( bis zu 700 ms)
ein "shalom" paket an den server, was er mit "shalom ok" oder 
"shalom client_constellation_changed" beantwortet,
sofern es im zeitlichen rahmen von 500 ms liegt,
falls nicht beendet sich der client.

bei "shalom client_constellation_changed"
erfragt der client mit "ask_for_clients" die liste des servers mit 
den mit ihm momentan verbundenen clients, was der server dann mit
"clients <MOMENTAN VERBUNDENE CLIENTS>" beantwortet.


	
