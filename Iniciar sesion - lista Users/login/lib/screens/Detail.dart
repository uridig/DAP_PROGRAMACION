import 'package:flutter/material.dart';
import './Home.dart';

class DetailScreen extends StatelessWidget {
  static const String name = 'detail';

  final Food food;

  DetailScreen({required this.food});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(food.title),
      ),
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            Container(
              width: 450, // Ancho completo del contenedor
              height: 450, // Ajusta la altura según tus necesidades
              child: Image.network(
                food.urlImage,
                fit: BoxFit.cover,
              ),
            ),
            SizedBox(height: 16),
            Text(
              food.title,
              style: Theme.of(context).textTheme.headline3?.copyWith(
                    fontSize:
                        32, // Ajusta el tamaño de la fuente según tus necesidades
                    fontWeight: FontWeight.bold,
                  ),
            ),
            SizedBox(height: 8),
            Text(
              food.description,
              style: Theme.of(context).textTheme.headline3?.copyWith(
                    fontSize:
                        20, // Ajusta el tamaño de la fuente según tus necesidades
                    fontWeight: FontWeight.bold,
                  ),
            ),
          ],
        ),
      ),
    );
  }
}
