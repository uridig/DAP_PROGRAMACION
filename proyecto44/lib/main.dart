import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

void main() {
  runApp(const MainApp());
}

class MainApp extends StatelessWidget {
  const MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: Scaffold( 
        body: Center(
          
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
             children: [
              Text('WELCOME', style: TextStyle(color: Color(0xFFFF0000)),),
              Row( 
                mainAxisAlignment: MainAxisAlignment.center,
                children: [Icon(Icons.person), Text('EITU y URI'),],
                ),
              Row(
                                mainAxisAlignment: MainAxisAlignment.center,
                                children: [Icon(Icons.key),Text('CONTRASEÑA'), ],

              )
              
              
          ],
          ),
        ),
      ),
    );
  }
}
