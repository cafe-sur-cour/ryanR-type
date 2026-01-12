<template>
  <div>
    <!-- Login Form -->
    <div v-if="!authenticated" class="min-h-screen bg-gray-900 flex items-center justify-center p-4">
      <div class="bg-gray-800 p-6 rounded-lg shadow-lg max-w-sm w-full">
        <h1 class="text-2xl font-bold text-center mb-6 text-white">R-Type Server Monitor</h1>

        <div class="space-y-4">
          <div>
            <label for="password" class="block text-sm font-medium text-gray-300 mb-2">
              Enter Password
            </label>
            <div class="relative">
              <Input
                id="password"
                v-model="password"
                :type="showPassword ? 'text' : 'password'"
                class="pr-10"
                placeholder="Password..."
                @keyup.enter="login"
              />
              <Button
                type="button"
                variant="ghost"
                size="icon"
                class="absolute inset-y-0 right-0 h-full px-3 hover:bg-transparent"
                @click="showPassword = !showPassword"
              >
                <EyeIcon v-if="!showPassword" class="h-4 w-4 text-gray-400" />
                <EyeOffIcon v-else class="h-4 w-4 text-gray-400" />
              </Button>
            </div>
          </div>
          <Button
            @click="login"
            :disabled="loading"
            class="w-full"
          >
            {{ loading ? 'Checking...' : 'Login' }}
          </Button>
        </div>
      </div>
    </div>

    <!-- Server Panel -->
    <div v-else class="min-h-screen bg-gray-900">
      <!-- Navigation Bar -->
      <nav class="bg-gray-800 border-b border-gray-700">
        <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
          <div class="flex justify-between h-16">
            <div class="flex">
              <div class="flex-shrink-0 flex items-center">
                <h1 class="text-xl font-bold text-white">R-Type Server Monitor</h1>
              </div>
              <div class="hidden sm:ml-6 sm:flex sm:space-x-8">
                <button
                  @click="activeTab = 'panel'"
                  :class="[
                    'border-transparent text-gray-300 hover:border-gray-300 hover:text-white px-1 pt-1 border-b-2 text-sm font-medium',
                    activeTab === 'panel' ? 'border-indigo-500 text-white' : ''
                  ]"
                >
                  Panel
                </button>
                <button
                  @click="activeTab = 'logs'"
                  :class="[
                    'border-transparent text-gray-300 hover:border-gray-300 hover:text-white px-1 pt-1 border-b-2 text-sm font-medium',
                    activeTab === 'logs' ? 'border-indigo-500 text-white' : ''
                  ]"
                >
                  Logs
                </button>
                <button
                  @click="activeTab = 'commands'"
                  :class="[
                    'border-transparent text-gray-300 hover:border-gray-300 hover:text-white px-1 pt-1 border-b-2 text-sm font-medium',
                    activeTab === 'commands' ? 'border-indigo-500 text-white' : ''
                  ]"
                >
                  Commands
                </button>
              </div>
            </div>
            <div class="flex items-center">
              <Button
                @click="logout"
                variant="ghost"
                size="sm"
                class="text-gray-300 hover:text-white"
              >
                Logout
              </Button>
            </div>
          </div>
        </div>
      </nav>

      <!-- Content -->
      <main class="flex-1">
        <ServerPanel
          v-if="activeTab === 'panel'"
          :password="password"
        />
        <div v-else-if="activeTab === 'logs'" class="p-6">
          <h2 class="text-2xl font-bold text-white mb-4">Server Logs</h2>
          <p class="text-gray-400">Logs functionality coming soon...</p>
        </div>
        <div v-else-if="activeTab === 'commands'" class="p-6">
          <h2 class="text-2xl font-bold text-white mb-4">Server Commands</h2>
          <p class="text-gray-400">Commands functionality coming soon...</p>
        </div>
      </main>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import ServerPanel from '../components/ServerPanel.vue'
import Button from '../components/ui/Button.vue'
import Input from '../components/ui/Input.vue'
import { EyeIcon, EyeOffIcon } from 'lucide-vue-next'

const authenticated = ref(false)
const password = ref('')
const loading = ref(false)
const showPassword = ref(false)
const activeTab = ref('panel')

const login = async () => {
  if (!password.value.trim()) return

  loading.value = true
  try {
    const response = await fetch('/api/status', {
      headers: {
        'Authorization': `Bearer ${password.value}`
      }
    })

    if (response.ok) {
      authenticated.value = true
    } else {
      alert('Invalid password')
    }
  } catch (error) {
    alert('Connection error')
  } finally {
    loading.value = false
  }
}

const logout = () => {
  authenticated.value = false
  password.value = ''
}
</script>

<style scoped>
</style>