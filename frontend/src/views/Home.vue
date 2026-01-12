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
    <ServerPanel
      v-else
      :password="password"
      @logout="logout"
    />
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