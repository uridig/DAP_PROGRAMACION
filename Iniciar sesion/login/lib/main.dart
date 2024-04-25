import 'package:flutter/material.dart';

void main() {
  runApp( MainApp());
}


class MainApp extends StatelessWidget {
   MainApp({Key? key});


  final TextEditingController userController = TextEditingController();
  final TextEditingController userController2 = TextEditingController();


  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              ElevatedButton(
                onPressed: () {
                  String ingresado = userController.text;
                  String ingresado2 = userController2.text;
                  if (ingresado == "Eitu" && ingresado2=="Cher") {
                    print("Funciona la app");
                  }
                  else {
                    print ("No funca");
                  }

                if (ingresado2!="cher")
                {
                  print("Contra mal");

                }
                if (ingresado!="Eitu")
                {
                  print("Usuario mal");

                }
                },
                child: const Text("LogIn"),
              ),
              TextField(
                decoration: const InputDecoration(
                border: OutlineInputBorder(),
                labelText: "Usuario",
                prefixIcon: (Icon(Icons.person)),


                ),
               
               
               
               
                controller: userController),

              TextField(
                decoration: const InputDecoration(
                border: OutlineInputBorder(),
                labelText: "Contraseña",
                prefixIcon: (Icon(Icons.key)),


                ),
               
               
               
               
                controller: userController2),
            ],
          ),
        ),
      ),
    );
  }
}




