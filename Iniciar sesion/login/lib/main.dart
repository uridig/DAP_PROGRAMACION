import 'package:flutter/material.dart';
import './screens/Router.dart';

void main() {
  runApp(MainApp());
}

class MainApp extends StatelessWidget {
  MainApp({super.key, Key});

  final TextEditingController userController = TextEditingController();
  final TextEditingController userController2 = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      routerConfig: RouterScreen,
    );
  }
}
