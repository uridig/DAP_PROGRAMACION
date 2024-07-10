import 'package:flutter_application_4/CORE/SCREENS/home_screens.dart';
import 'package:flutter_application_4/core/screens/login_screens.dart';
import 'package:go_router/go_router.dart';


final appRouter = GoRouter (
  initialLocation: '/login',
  routes: [
  GoRoute(path: '/login',
   builder: (context, state) => const LoginScreen(),
   ),

 GoRoute(
    name: HomeScreen.name,
    path: '/home',
    builder: (context, state) => HomeScreen(userName: state.extra as String),
  ),
]);