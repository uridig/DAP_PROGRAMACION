import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import './Home.dart';

class LoginScreen extends StatelessWidget {
  static const String name = 'login';

  const LoginScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: pantallaLogin(),
    );
  }
}

List<String> usuarios = ["Eitu", "Uri", "Ale"];
List<String> contrasenas = ["Dig", "Cher", "Capo"];

class pantallaLogin extends StatelessWidget {
  final TextEditingController userController = TextEditingController();
  final TextEditingController userController2 = TextEditingController();

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
                  if (usuarios.contains(ingresado) == true &&
                      contrasenas.contains(ingresado2) == true) {
                    print("TRUE");
                    print(usuarios.indexOf(ingresado));
                    print(contrasenas.indexOf(ingresado2));
                    if (usuarios.indexOf(ingresado) ==
                        contrasenas.indexOf(ingresado2)) {
                      print("Funciona la app");
                      context.pushNamed(HomeScreen.name, extra: ingresado);
                    } else {
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(
                          content: Text('No'),
                          duration: Duration(seconds: 2),
                        ),
                      );
                    }
                  } else {
                    ScaffoldMessenger.of(context).showSnackBar(
                      SnackBar(
                        content: Text('No'),
                        duration: Duration(seconds: 2),
                      ),
                    );
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
