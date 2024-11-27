import 'package:flutter/material.dart';
import 'TennisDataModel.dart';

class TennisDetail extends StatelessWidget {
  final TennisDataModel tennisDataModel;

  const TennisDetail({Key? key, required this.tennisDataModel})
      : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(tennisDataModel.name),
      ),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Image.network(tennisDataModel.imageUrl),
          SizedBox(height: 20),
          Text(
            tennisDataModel.description,
            textAlign: TextAlign.center,
            style: TextStyle(fontSize: 18),
          ),
        ],
      ),
    );
  }
}
