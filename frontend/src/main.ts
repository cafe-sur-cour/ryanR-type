import { createApp } from 'vue'
import { createRouter, createWebHistory } from 'vue-router'
import { createTheme, ThemeProvider } from '@mui/material/styles'
import CssBaseline from '@mui/material/CssBaseline'
import './style.css'
import App from './App.vue'
import Home from './views/Home.vue'

const routes = [
  { path: '/', component: Home },
]

const router = createRouter({
  history: createWebHistory(),
  routes,
})

const theme = createTheme({
  palette: {
    mode: 'dark',
    primary: {
      main: '#3b82f6', // blue-500
    },
    background: {
      default: '#111827', // gray-900
      paper: '#1f2937', // gray-800
    },
  },
})

const app = createApp(App)
app.use(router)
app.component('ThemeProvider', ThemeProvider)
app.provide('theme', theme)

// Add dark class to body for shadcn dark theme
document.body.classList.add('dark')

app.mount('#app')