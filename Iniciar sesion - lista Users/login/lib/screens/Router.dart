import 'package:go_router/go_router.dart';
import './Home.dart';
import './logIn.dart';
import './Detail.dart';

final RouterScreen = GoRouter(routes: [
  GoRoute(
    name: LoginScreen.name,
    path: '/',
    builder: (context, state) => const LoginScreen(),
  ),
  GoRoute(
    name: HomeScreen.name,
    path: '/home',
    builder: (context, state) => HomeScreen(nombreEitu: state.extra as String),
  ),
  GoRoute(
    name: DetailScreen.name,
    path: '/',
    builder: (context, state) => DetailScreen(food: state.extra as Food),
  ),
]);
