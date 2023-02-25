Nimi: Viivi Mustonen
Sähköposti: viivi.mustonen@tuni.fi
Opiskelijanumero: H293452

Toisessa vaiheessa on tietorakenteena suuntamaton ja painotettu graafi, joka on
luotu structilla Road. Road structia hyödynnetään jokaisen kaupungin structissa
Town, vektoriin on tallennettu kaikki kaupungista lähtevät tiet (jokaiseen tie
structiin pointer). Lisäksi kaikki tiet on tallennettu unordered_mappiin, jossa
tien id (muodostettu lähtö ja tulo kaupunkien id:stä) key:nä ja value:na Road
struct. Totuudessa graafin solmuina toimivat kaupungit ja niihin tallennettu
solmun tila.

Ajattelin ensin lisätä kaikki tiet map tietorakenteeseen, koska silloin mapin
key voisi olla std::pair, jossa on järjestetty town1 ja town2 pienemmän TownID:n
mukaan. KUitenkin map ei ole tehokkuus testissä ei niin tehokas, kuin
unordered_map. Siksi käytettiin tietorakenteeksi valittiin unordered_map ja
fuktiossa all_roads sortataan niin, että jokainen tie on listassa vain kerran,
ja esitetään parina kaupunki- id:itä, jotka esitetään siinä järjestyksessä, että
parin ensimmäinen id on pienempi kuin toinen id.

Projektissa minkä tahansa reitin hakemiseen käytetään depth-first-search(DFS).
DFS:ää on käytetty minkä tahansa reitin hakemiseen, koska sen tehokkuus
satunnaisen reitin löytämisessä on parempi kuin BFS.
Projektissa käytettiin breadth-first-search(BFS):iä least_towns_route:n hakemiseen.
BFS on tehokkain tapa löytää reitti jossa on mahdollisimman vähän solmuja
(kaupunkeja).
Myös any_routen olisi voinut toteuttaa BFS:llä, mutta koin että tehokkuuden ja
oppimisen kannalta on parempi toteuttaa kaksi eri funktiota eri haku algoritmailla.
Myös jos projektissa olisi toteutettu road_cycle_route, niin DFS oltaisiin
todennäköisesti hyödynnetty siinä joka tapauksessa.

Projektiin on toteutettu ja testattu luokalle Datastructures metodit clear_roads,
all_roads, add_road, get_roads_from, any_route, remove_road ja least_towns_route.
