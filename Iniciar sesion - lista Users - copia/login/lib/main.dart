import 'package:flutter/material.dart';
import './screens/Router.dart';

void main() {
  runApp(MainApp());
}

class MainApp extends StatelessWidget {
  MainApp({super.key, Key});

 

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      routerConfig: RouterScreen,
    );
  }
}
