import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'TennisDataModel.dart';
import 'TennisDetail.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'App Tenis',
      home: LoginScreen(),
    );
  }
}

class LoginScreen extends StatefulWidget {
  @override
  _LoginScreenState createState() => _LoginScreenState();
}

class _LoginScreenState extends State<LoginScreen> {
  final _emailController = TextEditingController();
  final _passwordController = TextEditingController();

  void _login() async {
    String email = _emailController.text.trim();
    String password = _passwordController.text.trim();

    final querySnapshot = await FirebaseFirestore.instance
        .collection('user')
        .where('email', isEqualTo: email)
        .where('password', isEqualTo: password)
        .get();

    if (querySnapshot.docs.isNotEmpty) {
      Navigator.push(
        context,
        MaterialPageRoute(builder: (context) => Tennis()),
      );
    } else {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Email o Contraseña es incorrecta')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Login'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            TextField(
              controller: _emailController,
              decoration: InputDecoration(labelText: 'Ingrese su Email'),
            ),
            TextField(
              controller: _passwordController,
              decoration: InputDecoration(labelText: 'Ingrese su Contraseña'),
              obscureText: true,
            ),
            SizedBox(height: 20),
            ElevatedButton(
              onPressed: _login,
              child: Text('Login'),
            ),
          ],
        ),
      ),
    );
  }
}

class Tennis extends StatefulWidget {
  const Tennis({Key? key}) : super(key: key);

  @override
  _TennisState createState() => _TennisState();
}

class _TennisState extends State<Tennis> {
  Future<List<Map<String, dynamic>>> fetchTennisData() async {
    final snapshot =
        await FirebaseFirestore.instance.collection('info-ten').get();

    return snapshot.docs.map((doc) {
      return {
        'id': doc.id,
        'name': doc['name'],
        'photo': doc['photo'],
        'gs': doc['gs'],
      };
    }).toList();
  }

  void _addNewTennisPlayer() {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        String name = '';
        String photo = '';
        String gs = '';

        return AlertDialog(
          title: Text('Agregar Nuevo Tenista'),
          content: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisSize: MainAxisSize.min,
            children: [
              TextField(
                onChanged: (value) {
                  name = value;
                },
                decoration: InputDecoration(labelText: 'Nombre del Tenista'),
              ),
              TextField(
                onChanged: (value) {
                  photo = value;
                },
                decoration: InputDecoration(labelText: 'URL de la Foto'),
              ),
              TextField(
                onChanged: (value) {
                  gs = value;
                },
                decoration: InputDecoration(labelText: 'Títulos de Grand Slam'),
                keyboardType: TextInputType.number,
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () {
                if (name.isEmpty || photo.isEmpty || gs.isEmpty) {
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(content: Text('Falta información')),
                  );
                  return;
                }

                FirebaseFirestore.instance.collection('info-ten').add({
                  'name': name,
                  'photo': photo,
                  'gs': gs,
                }).then((_) {
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(content: Text('Tenista agregado')),
                  );
                  Navigator.of(context).pop();
                  setState(() {});
                });
              },
              child: Text('Agregar'),
            ),
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: Text('Cancelar'),
            ),
          ],
        );
      },
    );
  }

  void _editTennisPlayer(Map<String, dynamic> tennisPlayer) {
    String name = tennisPlayer['name'];
    String photo = tennisPlayer['photo'];
    String gs = tennisPlayer['gs'];

    // Controladores de texto para mostrar los valores actuales y permitir la edición
    TextEditingController nameController = TextEditingController(text: name);
    TextEditingController photoController = TextEditingController(text: photo);
    TextEditingController gsController = TextEditingController(text: gs);

    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text('Editar Tenista'),
          content: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisSize: MainAxisSize.min,
            children: [
              TextField(
                controller: nameController,
                decoration: InputDecoration(
                  labelText: 'Nombre del Tenista',
                ),
              ),
              TextField(
                controller: photoController,
                decoration: InputDecoration(
                  labelText: 'URL de la Foto',
                ),
              ),
              TextField(
                controller: gsController,
                decoration: InputDecoration(
                  labelText: 'Títulos de Grand Slam',
                ),
                keyboardType: TextInputType.number,
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () async {
                // Verifica que los campos no estén vacíos antes de actualizar
                if (nameController.text.isEmpty ||
                    photoController.text.isEmpty ||
                    gsController.text.isEmpty) {
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(content: Text('Falta información')),
                  );
                  return;
                }

                // Actualiza en Firestore los datos del tenista
                await FirebaseFirestore.instance
                    .collection('info-ten')
                    .doc(tennisPlayer['id'])
                    .update({
                  'name': nameController.text,
                  'photo': photoController.text,
                  'gs': gsController.text,
                });

                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(content: Text('Tenista actualizado')),
                );

                Navigator.of(context).pop();
                setState(() {});
              },
              child: Text('Guardar'),
            ),
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: Text('Cancelar'),
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Tenistas'),
        actions: [
          IconButton(
            icon: Icon(Icons.add),
            onPressed: _addNewTennisPlayer,
          ),
        ],
      ),
      body: FutureBuilder<List<Map<String, dynamic>>>(
        future: fetchTennisData(),
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return Center(child: CircularProgressIndicator());
          }

          if (snapshot.hasError) {
            return Center(child: Text('Error: ${snapshot.error}'));
          }

          final tennisData = snapshot.data!;

          return ListView.builder(
            itemCount: tennisData.length,
            itemBuilder: (context, index) {
              final tennisPlayer = tennisData[index];

              return Card(
                child: ListTile(
                  title: Text(tennisPlayer['name']),
                  leading: SizedBox(
                    width: 50,
                    height: 50,
                    child: Image.network(tennisPlayer['photo']),
                  ),
                  trailing: Row(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      IconButton(
                        icon: Icon(Icons.edit),
                        onPressed: () => _editTennisPlayer(tennisPlayer),
                      ),
                      IconButton(
                        icon: Icon(Icons.delete),
                        onPressed: () {
                          // Confirmación antes de eliminar
                          showDialog(
                            context: context,
                            builder: (BuildContext context) {
                              return AlertDialog(
                                title: Text('Confirmar'),
                                content: Text(
                                    '¿Estás seguro de que quieres eliminar este tenista?'),
                                actions: [
                                  TextButton(
                                    onPressed: () async {
                                      await FirebaseFirestore.instance
                                          .collection('info-ten')
                                          .doc(tennisPlayer['id'])
                                          .delete();
                                      Navigator.of(context).pop();
                                      ScaffoldMessenger.of(context)
                                          .showSnackBar(SnackBar(
                                              content:
                                                  Text('Tenista eliminado')));
                                      setState(() {});
                                    },
                                    child: Text('Eliminar'),
                                  ),
                                  TextButton(
                                    onPressed: () {
                                      Navigator.of(context).pop();
                                    },
                                    child: Text('Cancelar'),
                                  ),
                                ],
                              );
                            },
                          );
                        },
                      ),
                    ],
                  ),
                  onTap: () {
                    Navigator.of(context).push(
                      MaterialPageRoute(
                        builder: (context) => TennisDetail(
                          tennisDataModel: TennisDataModel(
                            tennisPlayer['name'],
                            tennisPlayer['photo'],
                            '${tennisPlayer['name']} tiene ${tennisPlayer['gs']} títulos de Grand Slam a lo largo de su carrera',
                          ),
                        ),
                      ),
                    );
                  },
                ),
              );
            },
          );
        },
      ),
    );
  }
}
