import 'package:flutter/material.dart';

class HomeScreen extends StatelessWidget {
  static const String name = 'home';
  String nombreEitu;
  HomeScreen({super.key, this.nombreEitu=''});
  List<String> mostrar = ["River", "Anime", "Chocolates", "A", "B", "C", "D", "E", "F", "G"];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text('Home Screen'),
        ),
        body: Center(
          child:
              ListView.builder(
                itemCount: mostrar.length,
                itemBuilder: (context, index) {
                  return Card(
                    key: ValueKey(mostrar[index]),
                    child: Padding(
              padding: const EdgeInsets.all(10),
              child: Text(mostrar[index]),
            ),
                  );



                }
                )
            
        ));
  }
}
