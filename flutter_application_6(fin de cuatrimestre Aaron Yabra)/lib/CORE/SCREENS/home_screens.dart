import 'package:flutter_application_4/CORE/SCREENS/player_detail_screen.dart';
import 'package:flutter_application_4/entities/player.dart';
import 'package:flutter/material.dart';

class HomeScreen extends StatelessWidget {
  static const String name = 'home';
  final String userName;
  HomeScreen({super.key, this.userName = ''});
 
List <Player> players =[

 Player(
  title: 'Aaron Gordon', 
  desc: 'Aaron Addison Gordon es un jugador de baloncesto estadounidense que pertenece a la plantilla de los Denver Nuggets de la NBA. Con 2,03 metros de altura, juega en la posición de alero y ala-pívot', 
  imgurl: 'assets/Aarongordon.webp'),
  
 Player(
  title: 'LeBron James', 
  desc: 'LeBron Raymone James Sr. es un jugador de baloncesto estadounidense que pertenece a la plantilla de Los Angeles Lakers de la NBA. Con 2,06 metros de estatura, su posición es la de alero, pero su talento, versatilidad y poderío físico le permiten jugar tanto de base como de ala-pívot.', 
  imgurl: 'assets/LeBronJames.webp'),
  
 Player(
  title: 'Michael Jordan', 
  desc: 'Michael Jeffrey Jordan es un exjugador de baloncesto estadounidense. Con 1,98 metros de altura, jugaba en la posición de escolta. Es considerado por la mayoría de aficionados y especialistas como el mejor jugador de baloncesto de todos los tiempos.', 
  imgurl: 'assets/MichaelJordan.webp'),

 Player(
  title: 'Stephen Curry', 
  desc: 'Wardell Stephen Curry II, es un jugador de baloncesto estadounidense que pertenece a la plantilla de los Golden State Warriors de la NBA. Con 1,88 metros de altura, juega en la posición de base.', 
  imgurl: 'assets/StephenCurry.webp'),

 Player(
  title: 'Kobe Bryant', 
  desc: 'Kobe Bean Bryant fue un baloncestista estadounidense que jugaba en la posición de escolta. Disputó veinte temporadas en la NBA, todas ellas en Los Angeles Lakers. Hijo del exjugador de baloncesto Joe Bryant está considerado como uno de los mejores baloncestistas de todos los tiempos.', 
  imgurl: 'assets/KobeBryant.webp'),

 Player(
  title: 'Shaquille O Neal', 
  desc: 'Shaquille Rashaun O Neal es un exjugador de baloncesto estadounidense, que disputó 19 temporadas en la NBA. Con 2,16 metros de altura, jugaba en la posición de pívot.', 
  imgurl: 'assets/ShaquilleONeal.webp'),

 Player(
  title: 'James Harden', 
  desc: 'James Edward Harden, Jr es un jugador de baloncesto estadounidense que pertenece a la plantilla de Los Angeles Clippers de la NBA. Con 1,96 metros de altura, juega en la posición de base o escolta.', 
  imgurl: 'assets/JamesHarden.webp'),

 Player(
  title: 'Anthony Davis', 
  desc: 'Anthony Marshon Davis, Jr. es un jugador de baloncesto estadounidense que pertenece a la plantilla de Los Angeles Lakers de la NBA. Mide 2,08 metros y juega en la posición de ala-pívot.', 
  imgurl: 'assets/AnthonyDavis.webp'),

 Player(
  title: 'Jayson Tatum', 
  desc: 'Jayson Christopher Tatum es un baloncestista estadounidense que juega en los Boston Celtics de la NBA. Con 2,03 metros de estatura juega en la posición de alero.', 
  imgurl: 'assets/JaysonTatum.webp'),
 
 Player(
  title: 'Nikola Jokić', 
  desc: 'Nikola Jokić es un jugador de baloncesto serbio que pertenece a la plantilla de los Denver Nuggets de la NBA. Con 2,11 metros de estatura se desempeña en la posición de pívot. Desde que llegó a la NBA ha sido seis veces All-Star, tres veces MVP de la temporada y una vez MVP de las Finales.', 
  imgurl: 'assets/NikolaJokic.webp'),


] ;


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Players ranking'),
      ),
      body: ListView.builder(
        itemCount: players.length,
        itemBuilder: (context, index) {
          final player= players[index];
       return Card(
            margin: const EdgeInsets.all(10),
            child: InkWell(
                onTap: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) =>  PlayerDetailScreen(player: player,),
                  ),
                );
              },
              child: Padding(
              padding: const EdgeInsets.all(10.0),
              child: Row(
                children: [
                  Image.network(
                    player.imgurl,
                    width: 100,
                    height: 150,
                    fit: BoxFit.cover,
                  ),
                  const SizedBox(width: 10),
                  Expanded(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text(
                          player.title,
                          style: const TextStyle(
                            fontSize: 18,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                        const SizedBox(height: 10),
                        Text(
                          player.desc,
                          style: const TextStyle(
                            fontSize: 14,
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
            ),
          ));
        },
      ),
    );
   }
  }