<template>
  <div class="min-h-screen bg-gray-100 flex items-center justify-center">
    <div class="bg-white p-8 rounded-lg shadow-md max-w-md w-full">
      <h1 class="text-2xl font-bold text-center mb-6 text-gray-800">R-Type Server Status</h1>
      <div class="text-center mb-4">
        <div v-if="loading" class="animate-pulse">
          <div class="h-4 bg-gray-300 rounded w-3/4 mx-auto mb-4"></div>
          <div class="h-8 bg-gray-300 rounded w-1/2 mx-auto"></div>
        </div>
        <p v-else :class="status === 'Online' ? 'text-green-600' : 'text-red-600'" class="text-xl font-semibold">
          Server is {{ status }}
        </p>
      </div>
      <button @click="fetchStatus" class="w-full bg-blue-500 hover:bg-blue-600 text-white font-bold py-2 px-4 rounded transition duration-200">
        Refresh Status
      </button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'

const status = ref('')
const loading = ref(true)

const fetchStatus = async () => {
  loading.value = true
  try {
    const response = await fetch('/api/status')
    status.value = await response.text()
  } catch (error) {
    status.value = 'Error'
  } finally {
    loading.value = false
  }
}

onMounted(() => {
  fetchStatus()
})
</script>

<style scoped>
</style>