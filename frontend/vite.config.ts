import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { fileURLToPath, URL } from 'node:url'

export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  build: {
    outDir: 'dist',
    rollupOptions: {
      onwarn(warning, warn) {
        // Ignore les warnings "use client" de MUI
        if (warning.message.includes('Module level directives cause errors when bundled') &&
            warning.message.includes('"use client"')) {
          return
        }
        warn(warning)
      }
    }
  }
})