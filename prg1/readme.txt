Nimi: Viivi Mustonen
Sähköposti: viivi.mustonen@tuni.fi
Opiskelijanumero: H293452

Ajattelin että paras tapa kaupungin tietojen tallentamiseen on unordered_map,
jonka key arvoksi tulee kaupungin uniikki id ja valueksi Town-sturcti. Structiin
tallennetaan nimi, sijainti, verot, isäntäkaupunki ja vektori vasallikaupunkeja.

Unordered_map on asymptoottisesti tehokkaampi kuin tavallinen map, koska sen voi
järjestää missä järjestyksessä tahansa toisin kuin mapin. Unordered_mapista
key arvolla etsiminen ja myös unordered_mappiin lisääminen on käytännössä lähes
vakioaikaista. Structin puolestaan olisi voinut toteuttaa poitereiden avulla,
mutta koin että pointereiden käyttäminen ei välttämättä paranna ohjelman
tehokkuutta merkittävästi (ehkä muutamaa tapausta lukuunottamatta).

Funktioissa towns_alphabetically, towns_distance_increasing, min_distance,
max_distance ja towns_nearest käytetään hyväksi sort()-komentoa, joka muuttaa
funktioiden asymptoottiseksi tehokkuudeksi n log n. Std::sort on kaikista
tehokkain tapa järjestää vektori.

Rekursiivisten funktioiden find_master_town, find_vassal_town ja find_town_tax
tehokkuus on muihin funktioihin verrattuna asymptoottisesti huonompi ja riippuu
vahvasti siitä kuinka monta kerrosta rekursiota käydään läpi. Kuitenkin
mitä syvemmälle rekursiossa mennään sitä vähemmän on läpikäytäviä tasoja jäljellä,
tehokkuus on siis käytännössä log n.

