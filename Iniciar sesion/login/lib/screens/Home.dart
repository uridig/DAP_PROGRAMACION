import 'package:flutter/material.dart';

class HomeScreen extends StatelessWidget {
  static const String name = 'home';
  String nombreEitu = 'Eitu';

  HomeScreen({super.key, nombreEitu});
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text('Home Screen'),
        ),
        body: Center(
          child: Text('Bienvenido Eitu'),
        ));
  }
}
