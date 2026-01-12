<template>
  <div class="min-h-screen bg-gray-900 flex items-center justify-center p-4">
    <div class="bg-gray-800 p-6 rounded-lg shadow-lg max-w-sm w-full">
      <h1 class="text-2xl font-bold text-center mb-6 text-white">R-Type Server Panel</h1>

      <div class="space-y-4">
        <div class="text-center">
          <div v-if="loading" class="text-gray-400">Loading...</div>
          <div v-else class="text-lg font-semibold">
            <span :class="status === 'Online' ? 'text-green-400' : 'text-red-400'">
              Server: {{ status }}
            </span>
          </div>
        </div>
        <Button
          @click="refreshStatus"
          :disabled="loading"
          class="w-full"
          variant="outline"
        >
          Refresh
        </Button>
        <Button
          @click="$emit('logout')"
          class="w-full"
          variant="destructive"
        >
          Logout
        </Button>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import Button from './ui/Button.vue'

interface Props {
  password: string
}

const props = defineProps<Props>()

const emit = defineEmits<{
  logout: []
}>()

const status = ref('')
const loading = ref(false)

const refreshStatus = async () => {
  loading.value = true
  try {
    const response = await fetch('/api/status', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })

    if (response.ok) {
      status.value = await response.text()
    } else if (response.status === 401) {
      emit('logout')
    } else {
      status.value = 'Error'
    }
  } catch (error) {
    status.value = 'Error'
  } finally {
    loading.value = false
  }
}

// Auto refresh on mount
refreshStatus()
</script>

<style scoped>
</style>